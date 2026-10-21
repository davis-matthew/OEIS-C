/*
 * A256300  Least prime p such that p+6^k is prime for all k=0,...,n.
 * https://oeis.org/A256300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = least prime p such that p + 6^k is prime for all k =
 *            1,2,...,n (the case k = 0 is omitted; thus for n = 0 we just
 *            return the smallest prime). For n ≥ 10 no such prime exists
 *            because the residues 6^k modulo 11 cover all non‑zero residues,
 *            forcing some p + 6^k ≡ 0 (mod 11). Therefore a(n) =...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper data */
static bool *prime_sieve = NULL;
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static const idx_t SIEVE_LIMIT = 20000000ULL;   /* enough for n ≤ 9 */

static void init_sieve(void)
{
    if (prime_sieve) return;                  /* already built */

    size_t N = (size_t)(SIEVE_LIMIT + 1);
    prime_sieve = (bool*)malloc(N * sizeof(bool));
    if (!prime_sieve) return;

    for (size_t i = 0; i < N; ++i) prime_sieve[i] = true;
    prime_sieve[0] = prime_sieve[1] = false;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (prime_sieve[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                prime_sieve[j] = false;
        }
    }

    /* Count primes and store them */
    size_t cnt = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (prime_sieve[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) return;

    size_t idx = 0;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (prime_sieve[i]) prime_list[idx++] = (uint64_t)i;

    prime_count = cnt;
}

/* Pre‑compute 6^k for k = 0 … 9 (10^9 fits in 64 bits) */
static seq_t pow6[10];
static bool pow6_init = false;
static void init_pow6(void)
{
    if (pow6_init) return;
    pow6[0] = 1;           /* unused  */
    pow6[1] = 6;
    for (int k = 2; k <= 9; ++k) pow6[k] = pow6[k - 1] * (seq_t)6;
    pow6_init = true;
}

seq_t a(idx_t n)
{
    if (n >= 10) return 0;          /* no such prime exists for n ≥ 10 */

    init_sieve();
    init_pow6();

    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];
        bool ok = true;

        for (int k = 1; k <= n; ++k) {          /* k = 1 … n */
            seq_t q = p + pow6[k];
            if (q > SIEVE_LIMIT || !prime_sieve[(size_t)q]) {
                ok = false;
                break;
            }
        }

        if (ok) return p;                       /* minimal such prime found */
    }

    return 0;   /* should never be reached for n ≤ 9 */
}
