/*
 * A275945  Numbers n such that the average of different permutations of
 *          digits of n is an integer.
 * https://oeis.org/A275945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 210, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A275945 – Numbers n such that the average of all decimal
 * permutations of the digits of n is an integer.
 *
 * The average of the permutations equals
 *       S * R(k) / k
 * where S is the sum of digits of n, k is the number of digits, and
 * R(k) = (10^k – 1)/9 is the k‑digit repunit.
 * Thus the average is integral iff   k | S * R(k).
 *
 * The function a(idx_t n) returns the n‑th term (n ≥ 1) of the
 * sequence by brute‑forcing integers until n terms have been found.
 * Results are memoised for efficiency and for arbitrary order of
 * calls.  All arithmetic is performed using the seq_t type supplied
 * by the build system.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* ------------------------------------------------------------------ */
/* Test whether a single number satisfies the average‑integrality condition */
static bool
ok(seq_t v)
{
    if (v == 0)                /* n = 0 is not in the sequence          */
        return false;

    /* Determine the number of decimal digits k. */
    idx_t k = 0;
    seq_t tmp = v;
    while (tmp > 0) {
        tmp /= 10;
        ++k;
    }
    if (k == 0)
        return false;          /* should not happen, guard against 0 */

    /* Compute the sum of digits S. */
    seq_t tmp2 = v;
    idx_t sumfull = 0;
    while (tmp2 > 0) {
        seq_t d = tmp2 % 10;
        sumfull += (idx_t)d;
        tmp2 /= 10;
    }
    idx_t summod = sumfull % k;      /* S mod k */

    /* Compute repunit R(k) modulo k. */
    idx_t rep = 0;
    for (idx_t i = 0; i < k; ++i)
        rep = (rep * 10 + 1) % k;    /* rep = (rep*10+1) % k */

    /* The average is integral iff k divides S * R(k). */
    return ((summod * rep) % k) == 0;
}

/* ------------------------------------------------------------------ */
/* Main sequence generator */
seq_t
a(idx_t n)
{
    static seq_t *table = NULL;      /* memoised terms                          */
    static size_t cap   = 0;         /* allocated capacity of the table         */
    static idx_t  count = 0;         /* number of terms already stored          */
    static idx_t  cur   = 0;         /* last integer examined                   */

    if (n == 0)
        return 0;                    /* a(0) is undefined, return 0             */

    /* Initial allocation of the table. */
    if (!table) {
        cap   = 64;
        table = seq_alloc(cap);      /* allocates an array of seq_t objects     */
    }

    /* Find terms until we have at least n of them. */
    while (count < n) {
        ++cur;                         /* next natural number to test            */
        if (ok(cur)) {
            /* Grow the table if necessary. */
            if (count == cap) {
                size_t new_cap = cap << 1;
                table = seq_realloc(table, cap, new_cap);
                cap = new_cap;
            }
            table[count++] = cur;
        }
    }

    return table[n - 1];
}
