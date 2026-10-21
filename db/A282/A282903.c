/*
 * A282903  Concatenation of elements of P{d_1}, P{d_2}, P{d_3}, ..., P{d_n};
 *          where P{d_n} denote the power set of the set of divisors of n
 *          ordered by the size of the subsets, and in each subset, following
 *          the increasing order.
 * https://oeis.org/A282903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   _gen_divs, process_divisor_digits, process_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th digit of OEIS A282903.
   The sequence is obtained by writing, for each integer k = 1, 2, 3, …,
   the decimal digits of the numbers produced by concatenating the elements
   of every non‑empty subset of the divisors of k, subsets being ordered
   first by increasing size and then lexicographically.  The function
   returns the value of the digit at position n (1‑based). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_gen_divs.h"
#include "../utils/process_divisor_digits.h"
#include "../utils/process_size.h"

/* ----------------------------------------------------------------------
   Compare function for qsort (ascending uint64_t)
   ---------------------------------------------------------------------- */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t ia = *(const uint64_t *)a;
    uint64_t ib = *(const uint64_t *)b;
    return (ia < ib) ? -1 : (ia > ib) ? 1 : 0;
}

static void get_divisors(uint64_t n, uint64_t *divs, size_t *cnt)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    uint64_t tmp = n;

    /* factorisation (trial division) */
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        primes[pcnt] = tmp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    *cnt = 0;
    _gen_divs(1, 0, primes, exps, pcnt, divs, cnt);
    qsort(divs, *cnt, sizeof(uint64_t), cmp_uint64);
}

/* ----------------------------------------------------------------------
   Main function: a(n)
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t      count  = 0;
    seq_t      result = 0;
    bool       found  = false;

    for (uint64_t k = 1; !found; ++k) {
        uint64_t divs[256];
        size_t   dcnt = 0;
        get_divisors(k, divs, &dcnt);

        for (size_t s = 1; s <= dcnt && !found; ++s) {
            process_size(divs, dcnt, s, n, &count, &result, &found);
        }
    }
    return result;
}
