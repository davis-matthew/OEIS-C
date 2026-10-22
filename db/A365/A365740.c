/*
 * A365740  Length of the longest subsequence of {m: 1<=m<=n, m not prime} on
 *          which the Euler totient function phi A000010 is nondecreasing.
 * https://oeis.org/A365740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1020, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = length of the longest subsequence of
 * {m | 1 ≤ m ≤ n, m not prime} on which phi(m) is non‑decreasing.
 * The routine builds φ(m) for all 1 ≤ m ≤ n by a linear sieve
 * and then runs a patience‑sorting algorithm to find the
 * longest non‑decreasing subsequence of φ(m) over the non‑prime indices.
 */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t, idx_t are provided by the build system (not redefined here). */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Allocate arrays for φ and primality.  Index 0..n. */
    seq_t *phi = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!phi)
        return 0;   /* allocation failure */

    uint8_t *composite = (uint8_t *)malloc((n + 1) * sizeof(uint8_t));
    if (!composite) {
        free(phi);
        return 0;
    }
    /* zero the composite array; primes will be marked as 1. */
    for (idx_t i = 0; i <= n; ++i)
        composite[i] = 0;

    phi[0] = 0;    /* unused */
    phi[1] = 1;    /* φ(1) = 1 – 1 is treated as non‑prime */

    /* linear sieve to compute φ and mark composites */
    idx_t *prime_list = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!prime_list) {
        free(phi);
        free(composite);
        return 0;
    }
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (!composite[i]) {            /* i is prime */
            prime_list[prime_cnt++] = i;
            phi[i] = (seq_t)i - 1;      /* φ(p) = p − 1 */
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            idx_t p = prime_list[j];
            if (i > n / p)
                break;
            idx_t prod = i * p;
            composite[prod] = 1;
            if (i % p == 0) {
                phi[prod] = phi[i] * (seq_t)p;
                break;
            } else {
                phi[prod] = phi[i] * ((seq_t)p - 1);
            }
        }
    }
    free(prime_list);

    /* patience sorting for longest non‑decreasing subsequence */
    seq_t *tails = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!tails) {
        free(phi);
        free(composite);
        return 0;
    }
    size_t tl_len = 0;

    for (idx_t m = 1; m <= n; ++m) {
        if (m == 1 || composite[m]) {            /* non‑prime (including 1) */
            seq_t v = phi[m];
            /* binary search for first element > v (upper bound) */
            size_t lo = 0, hi = tl_len;
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (tails[mid] <= v)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            if (lo == tl_len)
                tails[tl_len++] = v;              /* extend sequence */
            else
                tails[lo] = v;                    /* improve ending value */
        }
    }

    seq_t result = (seq_t)tl_len;
    free(tails);
    free(phi);
    free(composite);
    return result;
}
