/*
 * A244576  Sum of all proper divisors of all positive integers <= prime(n).
 * https://oeis.org/A244576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2}^{p_n} (σ(k) – k – 1) where σ(k) is the sum of
 *            all divisors of k and p_n is the n‑th prime. This equals a(n) =
 *            S(p_n) – p_n*(p_n+1)/2 – (p_n – 1), where S(N) = Σ_{i=1..N} i *
 *            ⌊N/i⌋
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_cnt  = 0;       /* number of primes stored */

/* Make sure we have at least n primes in prime_list. */
static void
ensure_primes(size_t n)
{
    if (prime_cnt >= n)
        return;

    idx_t limit = _prime_limit_for_idx(n);
    uint64_t *tmp_primes;
    size_t    tmp_cnt;

    build_primes_upto(limit, &tmp_primes, &tmp_cnt);

    free(prime_list);          /* old array can be freed */
    prime_list = tmp_primes;
    prime_cnt  = tmp_cnt;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                 /* n must be ≥1 for the OEIS offset */

    ensure_primes((size_t)n);
    uint64_t p = prime_list[n - 1];   /* n‑th prime (pₙ) */

    seq_t N = (seq_t)p;

    /* Compute S(N) = Σ_{i=1..N} i * ⌊N/i⌋ */
    seq_t S = 0;
    seq_t i = 1;
    while (i <= N) {
        seq_t q    = N / i;        /* floor(N / i) */
        seq_t last = N / q;        /* last i with the same quotient q */
        seq_t count = last - i + 1;
        seq_t sum_range = (i + last) * count / 2;  /* Σ_{j=i}^{last} j */
        S += sum_range * q;
        i = last + 1;
    }

    seq_t sumk = N * (N + 1) / 2;  /* Σ_{k=1}^{N} k */
    seq_t result = S - sumk - (N - 1);
    return result;
}
