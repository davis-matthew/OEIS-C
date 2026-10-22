/*
 * A374600  If n = i^2 * A005117(j) for some i, j > 0 then a(n) = j^2 *
 *          A005117(i).
 * https://oeis.org/A374600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   If n = i^2 * A005117(j) then a(n) = j^2 * A005117(i)
 * Range:     u8 n <= 22, u16 n <= 418, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   uint64_pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/uint64_pow_uint.h"

/* Return true iff x is squarefree (no prime square divides x). */
static bool is_squarefree_uint64(uint64_t x)
{
    if (x == 0) return false;
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        uint64_t sq = p * p;
        if (x % sq == 0) return false;
    }
    return true;
}

/* Count the number of squarefree integers <= limit. */
static uint64_t count_squarefree_upto(uint64_t limit)
{
    uint64_t cnt = 0;
    for (uint64_t x = 1; x <= limit; ++x)
        if (is_squarefree_uint64(x))
            ++cnt;
    return cnt;
}

/* Return the k-th squarefree number (k ≥ 1). */
static uint64_t kth_squarefree(uint64_t k)
{
    uint64_t x = 0;
    uint64_t cnt = 0;
    while (cnt < k)
    {
        ++x;
        if (is_squarefree_uint64(x))
            ++cnt;
    }
    return x;
}

/* Computes the A374600 sequence element at index n (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* below offset; undefined behavior not required */

    uint64_t nn = (uint64_t)n;
    uint64_t i_val = 1;   /* sqrt part of the square factor (i) */
    uint64_t s_val = 1;   /* squarefree part (A005117(j)) */

    uint64_t tmp = nn;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p != 0) continue;
        uint64_t exp = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++exp;
        }
        uint64_t half = exp >> 1;           /* floor(exp/2) */
        if (half > 0)
            i_val *= uint64_pow_uint(p, half);
        if (exp & 1ULL)                     /* odd exponent contributes to squarefree part */
            s_val *= p;
    }

    /* Handle remaining prime factor, if any. */
    if (tmp > 1)
        s_val *= tmp;

    /* j is the index of s_val in the squarefree sequence. */
    uint64_t j_val = count_squarefree_upto(s_val);

    /* a_i is the i_val-th squarefree number. */
    uint64_t a_i = kth_squarefree(i_val);

    /* Compute result = j^2 * a_i. */
    seq_t result = (seq_t)j_val;
    result *= result;                /* j^2 */
    result *= (seq_t)a_i;

    return result;
}
