/*
 * A345147  a(1)=1. If a(n) is a novel term, a(n+1) = A000005(a(n)). If a(n)
 *          is a repeat term, a(n+1) = a(n)+m, where m is the least prior term
 *          (which once used in this way is not used again).
 * https://oeis.org/A345147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 484, u16 n <= 9731, u32 n <= 9731, u64 n <= 9731,
 *            u128 n <= 9731, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A345147
 * Recurrence: a(1) = 1.
 * If a(n) is novel, a(n+1) = d(a(n))   (number of divisors of a(n)).
 * If a(n) repeats, a(n+1) = a(n) + m,
 *   where m is the least value among all prior terms that have not yet
 *   been used as m, and the chosen term is then marked as used.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Compute the divisor count d(n) for a seq_t value */
static seq_t divisor_count(seq_t n) {
    seq_t count = 1;
    seq_t tmp   = n;
    seq_t d     = 2;
    /* iterate over possible prime divisors */
    while (d <= tmp / d) {            /* avoid overflow of d*d */
        if (tmp % d == 0) {
            seq_t exp = 0;
            while (tmp % d == 0) {
                tmp /= d;
                exp++;
            }
            count *= (exp + 1);
        }
        d++;
    }
    if (tmp > 1) {                     /* remaining prime factor */
        count *= 2;
    }
    return count;
}

seq_t a(idx_t n) {
    static seq_t *vals  = NULL;     /* 1‑based array of a(i) */
    static bool  *used_m = NULL;    /* true if index i has been used as m */
    static idx_t  max_n  = 0;        /* largest index computed */

    if (n < 1) return 0;            /* defined only for n ≥ 1 */

    /* Grow storage as needed */
    if (n > max_n) {
        size_t new_size = (size_t)(n + 1);   /* index 0 unused, 1…n used */
        vals  = (seq_t *)realloc(vals,  new_size * sizeof(seq_t));
        used_m = (bool  *)realloc(used_m, new_size * sizeof(bool));
        /* initialise new entries of used_m to false */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            used_m[i] = false;
        }

        /* Compute terms from max_n+1 up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i == 1) {                /* a(1) = 1 */
                vals[1] = 1;
                continue;
            }

            /* Determine if a(i-1) is novel */
            bool novel = true;
            for (idx_t j = 1; j < i - 1; ++j) {
                if (vals[j] == vals[i - 1]) {
                    novel = false;
                    break;
                }
            }

            if (novel) {
                /* a(i) = d(a(i-1)) */
                vals[i] = divisor_count(vals[i - 1]);
            } else {
                /* find minimal unused prior term value */
                seq_t best_val   = (seq_t)-1;  /* max value initialised */
                idx_t best_idx   = 0;
                bool   found     = false;
                for (idx_t j = 1; j < i - 1; ++j) {
                    if (!used_m[j]) {
                        if (!found || vals[j] < best_val) {
                            best_val = vals[j];
                            best_idx = j;
                            found    = true;
                        }
                    }
                }
                /* There must be at least one candidate */
                used_m[best_idx] = true;
                vals[i] = vals[i - 1] + best_val;
            }
        }
        max_n = n;
    }
    return vals[n];
}
