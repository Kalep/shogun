/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Written (W) 2012 Heiko Strathmann
 */

#include <shogun/base/init.h>
#include <shogun/mathematics/Statistics.h>
#include <shogun/features/streaming/generators/GaussianBlobsDataGenerator.h>
#include <gtest/gtest.h>

using namespace shogun;

TEST(GaussianBlobsDataGenerator,get_next_example)
{
	index_t num_blobs=1;
	float64_t distance=3;
	float64_t epsilon=2;
	float64_t angle=CMath::PI/4;
	index_t num_samples=10000;

	CGaussianBlobsDataGenerator* gen=new CGaussianBlobsDataGenerator(num_blobs,
			distance, epsilon, angle);

	/* two dimensional samples */
	SGMatrix<float64_t> samples(2, num_samples);

	for (index_t i=0; i<num_samples; ++i)
	{
		gen->get_next_example();
		SGVector<float64_t> sample=gen->get_vector();
		samples(0,i)=sample[0];
		samples(1,i)=sample[1];
		gen->release_example();
	}

	SGVector<float64_t> mean=CStatistics::matrix_mean(samples, false);
	SGMatrix<float64_t>::transpose_matrix(samples.matrix, samples.num_rows,
			samples.num_cols);
	SGMatrix<float64_t> cov=CStatistics::covariance_matrix(samples);
    //mean.display_vector("mean");
	//cov.display_matrix("cov");

    /* rougly ensures right results, set to 0.3 for now, if test fails, set a
     * bit larger */
    float64_t accuracy=0.3;

	/* matrix is expected to look like [1.5, 0.5; 0.5, 1.5] */
	EXPECT_LE(CMath::abs(cov(0,0)-1.5), accuracy);
	EXPECT_LE(CMath::abs(cov(0,1)-0.5), accuracy);
	EXPECT_LE(CMath::abs(cov(1,0)-0.5), accuracy);
	EXPECT_LE(CMath::abs(cov(1,1)-1.5), accuracy);
	
	/* mean is supposed to do [0, 0] */
	EXPECT_LE(CMath::abs(mean[0]-0), 0.1);
	EXPECT_LE(CMath::abs(mean[1]-0), 0.1);

	/* and another one */
	SGMatrix<float64_t> samples2(2, num_samples);
	num_blobs=3;
	gen->set_blobs_model(num_blobs, distance, epsilon, angle);

	for (index_t i=0; i<num_samples; ++i)
	{
		gen->get_next_example();
		SGVector<float64_t> sample=gen->get_vector();
		samples2(0,i)=sample[0];
		samples2(1,i)=sample[1];
		gen->release_example();
	}

	SGVector<float64_t> mean2=CStatistics::matrix_mean(samples2, false);
	SGMatrix<float64_t>::transpose_matrix(samples2.matrix, samples2.num_rows,
			samples2.num_cols);
	SGMatrix<float64_t> cov2=CStatistics::covariance_matrix(samples2);
    //mean2.display_vector("mean2");
	//cov2.display_matrix("cov2");


	/* matrix is expected to look like [7.55, 0.55; 0.55, 7.55] */
	EXPECT_LE(CMath::abs(cov2(0,0)-7.55), accuracy);
	EXPECT_LE(CMath::abs(cov2(0,1)-0.55), accuracy);
	EXPECT_LE(CMath::abs(cov2(1,0)-0.55), accuracy);
	EXPECT_LE(CMath::abs(cov2(1,1)-7.55), accuracy);
	
	/* mean is supposed to do [3, 3] */
	EXPECT_LE(CMath::abs(mean2[0]-3), accuracy);
	EXPECT_LE(CMath::abs(mean2[1]-3), accuracy);

	SG_UNREF(gen);
}
