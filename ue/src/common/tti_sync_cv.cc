/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2015 The srsUE Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution.
 *
 * \section LICENSE
 *
 * This file is part of the srsUE library.
 *
 * srsUE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsUE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#include <pthread.h>

#include "common/tti_sync_cv.h"


namespace srsue {

  tti_sync_cv::tti_sync_cv(uint32_t modulus): tti_sync(modulus)
  {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
  }

  tti_sync_cv::~tti_sync_cv()
  {
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
  }

  uint32_t tti_sync_cv::wait()
  {
    pthread_mutex_lock(&mutex);
    while(wait_condition()) {
      pthread_cond_wait(&cond, &mutex);
    }
    uint32_t x = consumer_cntr;
    increase_consumer();
    pthread_mutex_unlock(&mutex);
    return x;
  }

  void tti_sync_cv::resync()
  {
    consumer_cntr = producer_cntr;
  }

  void tti_sync_cv::set_producer_cntr(uint32_t producer_cntr)
  {
    pthread_mutex_lock(&mutex);
    init_counters(producer_cntr);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }

  void tti_sync_cv::increase()
  {
    pthread_mutex_lock(&mutex);
    increase_producer();
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }
}