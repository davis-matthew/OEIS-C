/*
 * A306300  Discriminant D of real quadratic number field Q(sqrt(D))
 *          associated with fundamental discriminant d = A003658(n).
 * https://oeis.org/A306300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A306300:
 * a(n) = A007913(A003658(n))
 * Fundamental discriminant d = A003658(n)
 * D = a(n) = d / largest_square_divisor(d)
 * For d ≡ 1 (mod 4): largest square divisor = 1,  a(n) = d
 * For d ≡ 0 (mod 4) with d/4 ≡ 2 or 3 (mod 4): largest square divisor = 4,
 *                              a(n) = d/4
 * The implementation enumerates fundamental discriminants in increasing order
 * and returns the nth one divided by its largest square divisor.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether x is square‑free (assumes x <= 2^64). */
static bool is_squarefree_u64(uint64_t x)
{
    if (x < 2) return true;   /* 0 and 1 are considered square‑free */

    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            uint64_t p2 = p * p;
            if (x % p2 == 0) return false;      /* p² divides x -> not square‑free */
            /* remove all powers of p so that p² never again divides the remaining x */
            while (x % p == 0) x /= p;
        }
    }
    return true;
}

/* Return the nth term of sequence A306300. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1, but safe */

    idx_t count = 0;
    uint64_t m = 1;

    while (true)
    {
        /* Case 1: d ≡ 1 (mod 4),  d must be square‑free */
        if ((m & 3ULL) == 1ULL)
        {
            if (is_squarefree_u64(m))
            {
                ++count;
                if (count == n) return (seq_t)m;   /* a(n) = d itself */
            }
        }
        /* Case 2: d ≡ 0 (mod 4),  d/4 must be square‑free and ≡ 2 or 3 (mod 4) */
        else if ((m & 3ULL) == 0ULL)
        {
            uint64_t q = m >> 2;                /* q = d / 4 */
            /* q must be ≡ 2 or 3 (mod 4); q = 1 or ≡ 0 (mod 4) are invalid */
            uint64_t qmod4 = q & 3ULL;
            if ((qmod4 == 2ULL) || (qmod4 == 3ULL))
            {
                if (is_squarefree_u64(q))
                {
                    ++count;
                    if (count == n) return (seq_t)q;   /* a(n) = d / 4 */
                }
            }
        }
        ++m;
    }
}
