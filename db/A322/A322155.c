/*
 * A322155  Consecutive terms that appear more than once in A014237.
 * https://oeis.org/A322155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n-th distinct value v that occurs in consecutive runs
 *            (length >= 2) in A014237, A014237(k) = prime(k) - k-th nonprime;
 *            found by sieve scan
 * Range:     i8 n <= 5, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    size_t limit = 1 << 16;
    for (;;) {
        char *comp = (char *)calloc(limit + 2, 1); /* 1 = nonprime */
        if (!comp) return 0;
        comp[0] = 1; comp[1] = 1;
        for (size_t i = 2; i * i <= limit; i++)
            if (!comp[i])
                for (size_t j = i * i; j <= limit; j += i) comp[j] = 1;
        /* 1 counts as nonprime (comp[1]=1); 0 is skipped by starting q at 1 */
        size_t p = 2, q = 1;
        long long prev = 0;
        bool have_prev = false, counted = false;
        idx_t count = 0;
        bool done = false;
        long long result = 0;
        while (p <= limit) {
            long long val = (long long)p - (long long)q;
            if (have_prev && val == prev) {
                if (!counted) {
                    counted = true;
                    count++;
                    if (count == n) { result = val; done = true; break; }
                }
            } else {
                counted = false;
            }
            prev = val;
            have_prev = true;
            p++;
            while (p <= limit && comp[p]) p++;
            q++;
            while (q <= limit && !comp[q]) q++;
        }
        free(comp);
        if (done) return (seq_t)result;
        limit *= 2;
    }
}
