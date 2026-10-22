/*
 * A306467  Let S(n)_k be the smallest positive integer t that t!k is a
 *          multiple of n (t!k is k-tuple factorial of t); then a(n) is the
 *          smallest k for which S(n)_k = n.
 * https://oeis.org/A306467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k for which the minimal t with n dividing
 *            the k‑tuple factorial of t (t!k) equals n. The k‑tuple factorial
 *            t!k is defined by t!k = t · (t−k) · (t−2k) · … (product of all
 *            positive integers in the arithmetic progression with difference
 *            k)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* standard headers for integer types */
#include <stdint.h>
#include <stdlib.h>

/* helper macro for 128‑bit multiplication modulo mod (uint64_t) */
#define MULMOD128(a,b,mod) ((uint64_t)(((__uint128_t)(a) * (b)) % (mod)))

/* sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based, but may be 0 for completeness */
    if (n <= 1u)   /* a(1) = 1 and a(0) is undefined but harmless */
        return (seq_t)1;

    uint64_t N = (uint64_t)n;   /* index fits into 64 bits */
    uint64_t *prod = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    if (!prod) return (seq_t)0;   /* allocation failure, return 0 */

    /* k iterates from 1 up to N (k = N is guaranteed to work) */
    for (uint64_t k = 1; k <= N; ++k)
    {
        uint64_t mod = N;
        uint64_t found_t = N + 1;   /* sentinel, not found */

        for (uint64_t t = 1; t <= N; ++t)
        {
            uint64_t val;
            if (t <= k)
                val = t % mod;
            else
                val = MULMOD128(prod[t - k], t, mod);

            prod[t] = val;

            if (val == 0)
            {
                found_t = t;
                break;   /* minimal t found for this k */
            }
        }

        if (found_t == N)   /* S(n)_k = n achieved */
        {
            free(prod);
            return (seq_t)k;
        }
    }

    /* fallback (should not be reached) */
    free(prod);
    return (seq_t)N;
}
