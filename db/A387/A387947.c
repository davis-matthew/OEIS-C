/*
 * A387947  Number of prime pairs {p, q} with n >= p > q such that (p-1)^2 ==
 *          (q-1)^2 (mod n).
 * https://oeis.org/A387947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered prime pairs {p,q} with * n ≥ p > q, p
 *            and q primes, * such that (p−1)² ≡ (q−1)² (mod n)
 * Range:     u8 n <= 479, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Count of prime pairs satisfying the congruence modulo n. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Obtain all primes ≤ n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Count how many primes produce each residue r = (p-1)² mod n. */
    size_t sz = (size_t)n;                     /* n is small (≤ 10000) */
    seq_t *res_cnt = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!res_cnt)
    {
        free(primes);
        return 0;                              /* allocation failed */
    }

    seq_t total = 0;                           /* result */

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p    = primes[i];
        uint64_t mod  = (p - 1) % n;            /* (p-1) mod n */
        uint64_t r    = (mod * mod) % n;        /* (p-1)² mod n */

        seq_t c = res_cnt[r];                   /* primes already seen with this residue */
        total += c;                            /* each such prime forms a pair with p */
        res_cnt[r] = c + 1;                    /* record p for future pairs */
    }

    free(primes);
    free(res_cnt);
    return total;
}
