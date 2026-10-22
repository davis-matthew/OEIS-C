/*
 * A324514  Number of aperiodic permutations of {1..n}.
 * https://oeis.org/A324514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * (n/d)^d * d! (OEIS A324514)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/factorial.h"

/* Small primes up to 97 – sufficient for factorising numbers ≤ 10 000. */
static const uint32_t SMALL_PRIMES[] = {
    2u, 3u, 5u, 7u, 11u, 13u, 17u, 19u, 23u, 29u,
    31u, 37u, 41u, 43u, 47u, 53u, 59u, 61u, 67u, 71u,
    73u, 79u, 83u, 89u, 97u
};
#define NUM_SMALL_PRIMES (sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]))

/* ---- Mobius function for numbers up to 10 000 --------------------------------- */
static int mobius_of(idx_t m)
{
    if (m == 1) return 1;
    int cnt = 0;
    idx_t tmp = m;
    for (size_t i = 0; i < NUM_SMALL_PRIMES && (idx_t)SMALL_PRIMES[i] * SMALL_PRIMES[i] <= tmp; ++i) {
        uint32_t p = SMALL_PRIMES[i];
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }
        if (exp) {
            if (exp > 1) return 0;       /* square factor → μ = 0 */
            cnt++;
        }
    }
    if (tmp > 1) cnt++;                    /* remaining prime factor */
    return (cnt & 1) ? -1 : 1;
}

/* ---- Generate all divisors of n given its prime factorisation ---------------- */
static void gen_divisors(const idx_t *prime, const int *exp,
                         int pos, idx_t prod, idx_t *out, size_t *cnt, int num_primes)
{
    if (pos == num_primes) {
        out[(*cnt)++] = prod;
        return;
    }
    idx_t val = prod;
    for (int e = 0; e <= exp[pos]; ++e) {
        gen_divisors(prime, exp, pos + 1, val, out, cnt, num_primes);
        val *= prime[pos];
    }
}

/* ---- Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, handled safely */
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    /* Factorise n (smallish, so simple trial division suffices). */
    idx_t n_primes[NUM_SMALL_PRIMES];
    int  n_exps [NUM_SMALL_PRIMES];
    int  n_cnt   = 0;
    idx_t tmp    = n;
    for (size_t i = 0; i < NUM_SMALL_PRIMES && (idx_t)SMALL_PRIMES[i] * SMALL_PRIMES[i] <= tmp; ++i) {
        uint32_t p = SMALL_PRIMES[i];
        int exp = 0;
        while (tmp % p == 0) {
            tmp /= p;
            exp++;
        }
        if (exp) {
            n_primes[n_cnt] = p;
            n_exps  [n_cnt] = exp;
            n_cnt++;
        }
    }
    if (tmp > 1) {                         /* remaining prime factor > 97 */
        n_primes[n_cnt] = (uint32_t)tmp;
        n_exps  [n_cnt] = 1;
        n_cnt++;
    }

    /* Enumerate all divisors of n.  n ≤ 10 000 ⇒ at most 256 divisors. */
    idx_t divisors[256];
    size_t div_cnt = 0;
    gen_divisors(n_primes, n_exps, 0, (idx_t)1, divisors, &div_cnt, n_cnt);

    /* Accumulate the Mobius‑weighted sum. */
    seq_t result = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        idx_t d = divisors[i];
        idx_t m = n / d;                   /* n/d */
        int mu = mobius_of(m);
        if (mu == 0) continue;             /* contributes nothing */

        seq_t pow_term   = big_pow((seq_t)m, (seq_t)d);
        seq_t fact_term  = factorial((idx_t)d);
        seq_t term       = pow_term * fact_term;

        if (mu > 0) result += term;
        else        result -= term;         /* mu = -1, subtract */
    }
    return result;
}
