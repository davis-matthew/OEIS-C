/*
 * A307646  Numbers that are the sum of a prime number and a nonnegative cube.
 * https://oeis.org/A307646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A307646:
 * Numbers that are the sum of a prime and a non‑negative cube.
 * The function builds a sieve up to a dynamically chosen bound,
 * marks all representable numbers, and scans them in ascending
 * order until the requested index is reached.  It works for any
 * seq_t/idx_t combination supplied by the build environment. */

#include <stdlib.h>
#include <string.h>

/* External types supplied by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety, OEIS offset starts at 1 */

    size_t target = (size_t)n;       /* zero‑based target index */
    size_t bound  = target * 10;     /* initial bound guess */

    for (;;) {
        /* ---------- Build sieve of primes up to 'bound' ---------- */
        char *is_prime = (char *)malloc(bound + 1);
        if (!is_prime) return 0;                    /* memory failure */
        memset(is_prime, 1, bound + 1);
        if (bound >= 0) is_prime[0] = 0;
        if (bound >= 1) is_prime[1] = 0;

        for (size_t i = 2; i * i <= bound; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= bound; j += i) {
                    is_prime[j] = 0;
                }
            }
        }

        /* Count primes to allocate array */
        size_t prime_cnt = 0;
        for (size_t i = 2; i <= bound; ++i) {
            if (is_prime[i]) prime_cnt++;
        }
        uint64_t *primes = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
        if (!primes) { free(is_prime); return 0; }

        /* Populate primes array */
        size_t idx = 0;
        for (size_t i = 2; i <= bound; ++i) {
            if (is_prime[i]) primes[idx++] = i;
        }

        /* ---------- Mark representable numbers ---------- */
        char *mark = (char *)calloc(bound + 1, 1);
        if (!mark) { free(is_prime); free(primes); return 0; }

        /* Compute maximum cube root within bound */
        size_t cube_base = 0;
        while ((cube_base + 1) * (cube_base + 1) * (cube_base + 1) <= bound) {
            cube_base++;
        }

        for (size_t c = 0; c <= cube_base; ++c) {
            idx_t cube = (idx_t)c * c * c;
            if (cube > bound) break;
            idx_t maxp = bound - cube;

            /* Add p + cube for all primes <= maxp */
            for (size_t pi = 0; pi < prime_cnt && primes[pi] <= (uint64_t)maxp; ++pi) {
                idx_t sum = (idx_t)primes[pi] + cube;
                if (sum <= bound) mark[(size_t)sum] = 1;
            }
        }

        /* ---------- Find the target term ---------- */
        size_t matched = 0;
        seq_t result = 0;
        for (size_t val = 2; val <= bound; ++val) {
            if (mark[val]) {
                matched++;
                if (matched == target) {
                    result = (seq_t)val;
                    break;
                }
            }
        }

        /* Clean up */
        free(is_prime);
        free(primes);
        free(mark);

        if (matched >= target) {
            return result;
        }

        /* Not enough terms yet – enlarge bound and retry */
        bound *= 2;
    }
}
