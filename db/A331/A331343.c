/*
 * A331343  a(n) = lcm(1,2,...,n) * Sum_{k=1..n} (2^(k-1) - 1) / k.
 * https://oeis.org/A331343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(1,2,...,n) * Sum_{k=1..n} (2^(k-1)-1)/k
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * The function implements the definition directly:  
 *   L = lcm(1..n)   (product of p^e for each prime p≤n)  
 *   a(n) = Σ_{k=1..n} (L/k) * (2^(k-1)-1)
 *
 * No auxiliary libraries are used; everything is computed with
 * standard C/C++ integer arithmetic.  The algorithm is O(n log log n)
 * for the sieve and O(n) for the summation, easily fast enough for n≤10000.
 */

#include <stdint.h>
#include <stdlib.h>

/* External types defined by the build system */
extern seq_t a(idx_t n)
{
    if (n == 0)  /* for safety – offset is 1, but the formula works for n=0 too */
        return (seq_t)0;

    /* --------- compute L = lcm(1..n) using prime powers --------- */
    idx_t limit = n;
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_prime)
        return (seq_t)0;  /* fallback on allocation failure */

    for (idx_t i = 0; i <= limit; ++i)
        is_prime[i] = 1;
    if (limit >= 0)
        is_prime[0] = 0;
    if (limit >= 1)
        is_prime[1] = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    seq_t L = 1;
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p])
            continue;
        /* largest power of p not exceeding limit */
        idx_t pow = p;
        while (1) {
            idx_t next = pow * p;
            if (next > limit)
                break;
            pow = next;
        }
        seq_t p_pow = (seq_t)pow;   /* p^e fits into seq_t because pow ≤ limit */
        L *= p_pow;                 /* L = lcm(1..n) */
    }
    free(is_prime);

    /* --------- compute the sum of terms --------- */
    seq_t sum = 0;
    seq_t pow2 = 1;  /* 2^(k-1) for current k, starting with k=1 */

    for (idx_t k = 1; k <= limit; ++k) {
        seq_t term_coeff = L / (seq_t)k;          /* L/k is an integer */
        seq_t term = term_coeff * (pow2 - 1);     /* (2^(k-1)-1) */
        sum += term;
        pow2 *= 2;                                /* advance pow2 to 2^k */
    }

    return sum;
}
