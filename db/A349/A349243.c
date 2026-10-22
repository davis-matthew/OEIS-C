/*
 * A349243  Indices of triangular numbers A000217 with only odd digits.
 * https://oeis.org/A349243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function computes A349243: the indices n (starting at 1) for which
 *  the triangular number T(n) = n*(n+1)/2 consists only of odd decimal digits.
 *  The implementation uses a lazy cache that expands up to a fixed maximum.
 *  The search checks each candidate n, computes T(n), and verifies that every
 *  decimal digit of T(n) is odd.  No dynamic allocation of seq_t objects is
 *  used; a static array works for both scalar and arbitrary‑precision seq_t.
 *
 *  The function returns 0 for n < 1, but this value is undefined for the
 *  sequence (the prescribed offset is 1).
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Adjust the cache size if larger values of n are required. */
#if !defined(MAX_CACHE)
#  define MAX_CACHE 200000
#endif

seq_t a(idx_t n)
{
    /* static cache of found indices */
    static seq_t indices[MAX_CACHE];
    static size_t cache_len = 0;        /* number of indices stored */
    static idx_t next_candidate = 1;    /* next n to test */

    if (n < 1) return 0;  /* undefined for n=0 but return 0 */

    /* ensure we have at least n entries cached */
    if (cache_len < n) {
        /* guard against exceeding the predefined cache size */
        if (cache_len + (n - cache_len) > MAX_CACHE) {
            /* if the cache would overflow, abort: useful during debugging */
            abort();
        }

        /* extend the cache to the required size */
        while (cache_len < n) {
            seq_t curr = (seq_t)next_candidate;
            seq_t tri = (curr * (curr + 1u)) / 2u;

            /* check if all decimal digits of tri are odd */
            seq_t tmp = tri;
            bool ok = (tmp > 0);          /* reject tri == 0 (digit 0) */
            while (ok && tmp > 0) {
                seq_t digit = tmp % 10u;   /* obtain last decimal digit */
                if ((digit & 1u) == 0u) {  /* even digit found */
                    ok = false;
                }
                tmp /= 10u;               /* drop last digit */
            }

            if (ok) {
                indices[cache_len++] = next_candidate;
            }
            ++next_candidate;
        }
    }

    return indices[n - 1];
}
