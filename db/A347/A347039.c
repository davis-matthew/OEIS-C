/*
 * A347039  Times on the display of a 24-hour digital clock with 6 digits,
 *          rounded to full seconds, at which the hour and minute hands of an
 *          analog clock form a right angle. Terms with fewer than 6 digits
 *          are to be assumed filled with zeros to the left.
 * https://oeis.org/A347039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   encode_time
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A347039.  The times of day (rounded to the nearest second) when the
   hour and minute hands of an analog clock form a right angle are given by
   the formulas
        t90  = ((10800 + 43200*k) + 5) / 11   (k = 0,1,2,…)
        t270 = ((32400 + 43200*k) + 5) / 11   (k = 0,1,2,…)
   where the division is integer division after adding 5 to round to the
   nearest second.  These times (in seconds) are then converted to the
   6‑digit HHMMSS representation with leading zeros omitted.
   The list repeats every 44 terms (once per day).  */

#ifndef OEIS_A347039_H
#define OEIS_A347039_H

#include <stdbool.h>   /* bool, true, false */
#include <stddef.h>    /* size_t */
#include "../utils/encode_time.h"

seq_t a(idx_t n)
{
    /* static cache of the 44 daily times */
    static seq_t times[44];
    static bool init = false;
    static idx_t count = 0;

    if (!init) {
        idx_t c = 0;
        /* Generate all times < 86400 seconds (i.e. < 24h) */
        for (idx_t k = 0; k <= 21; ++k) {
            /* 90° solutions */
            idx_t num90 = 10800 + 43200 * k;
            seq_t s90 = (seq_t)(num90 + 5) / 11;
            if (s90 < 86400) {
                times[c++] = encode_time(s90);
            }
            /* 270° solutions */
            idx_t num270 = 32400 + 43200 * k;
            seq_t s270 = (seq_t)(num270 + 5) / 11;
            if (s270 < 86400) {
                times[c++] = encode_time(s270);
            }
        }
        count = c;   /* count should be 44 */

        /* Simple insertion sort (size is tiny) */
        for (idx_t i = 0; i < count; ++i) {
            for (idx_t j = i + 1; j < count; ++j) {
                if (times[i] > times[j]) {
                    seq_t tmp = times[i];
                    times[i] = times[j];
                    times[j] = tmp;
                }
            }
        }
        init = true;
    }

    /* the sequence repeats every 'count' terms (once per day) */
    return times[(n - 1) % count];
}

#endif /* OEIS_A347039_H */
