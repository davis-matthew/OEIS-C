/*
 * A368074  a(n) is the least number k such that the k-th n-gonal number can
 *          be written as the difference of two positive n-gonal numbers in
 *          exactly n ways.
 * https://oeis.org/A368074
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes OEIS A368074: a(n) = the least k such that the k‑th n‑gonal number
 * can be expressed as the difference of two positive n‑gonal numbers in
 * exactly n ways.
 *
 * The algorithm:
 *   * For each n ≥ 3, search k = 1,2,… until the number of representations
 *     equals n.
 *   * For a fixed (k,n) we use the identity
 *
 *        P_n(k) = ((n‑2)k² – (n‑4)k)/2
 *
 *     Let A = 2·P_n(k).  All representations correspond to divisors d of A.
 *     For a divisor d we compute
 *         B = A / d
 *     and test whether there exists an integer i ≥ 1 such that
 *         B + (n‑4) = M·(2i + d)      with M = n‑2.
 *
 *     This reduces to
 *         RHS = (B + (n‑4)) / M
 *         i   = (RHS – d)/2
 *
 *     The count of divisors d for which the above holds and i ≥ 1 equals
 *     the desired number of representations.
 *
 *   * To find all divisors we factor A by trial division using a pre‑computed
 *     sieve of primes up to 2·10⁶ (enough for n ≤ 10 000).
 *
 * The function works for any seq_t chosen by the build (8/16/32/64/128‑bit
 * unsigned integers or arbitrary‑precision unsigned integers).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRIME 2000000U  // enough for n ≤ 10 000

/* --- global prime table ------------------------------------------- */
static seq_t *global_primes = NULL;   /* array of primes (seq_t type) */
static size_t global_prime_count = 0; /* number of primes in the array */

/* --- initialize primes using Sieve of Eratosthenes ------------------ */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    uint8_t *is_prime = (uint8_t *)calloc(MAX_PRIME + 1, sizeof(uint8_t));
    if (!is_prime) return;                // silently fail on low memory

    memset(is_prime + 2, 1, MAX_PRIME - 1); // exclude indices 0,1

    for (uint32_t i = 2; (uint64_t)i * i <= MAX_PRIME; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) ++cnt;

    global_primes = (seq_t *)malloc(cnt * sizeof(seq_t));
    if (!global_primes) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (uint32_t i = 2; i <= MAX_PRIME; ++i)
        if (is_prime[i]) global_primes[idx++] = (seq_t)i;

    global_prime_count = cnt;
    free(is_prime);
}

/* --- prime factorisation of a seq_t number ------------------------- */
static void factorize(seq_t n,
                      const seq_t *primes, size_t primes_cnt,
                      size_t *pcnt, seq_t *fac_primes, size_t *fac_exps)
{
    size_t cnt = 0;
    seq_t temp = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        seq_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            fac_primes[cnt] = p;
            fac_exps[cnt]   = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        fac_primes[cnt] = temp;
        fac_exps[cnt]   = 1;
        ++cnt;
    }
    *pcnt = cnt;
}

/* --- recursive divisor enumeration and representation counting ----- */
static void enumerate_divisors(const seq_t *primes, const size_t *exps,
                               size_t pcnt, seq_t current, seq_t A,
                               seq_t M, seq_t n, size_t *count)
{
    if (pcnt == 0) {
        seq_t d = current;
        seq_t B = A / d;

        seq_t temp;
        if (n >= (seq_t)4)
            temp = B + (n - (seq_t)4);   /* n‑4 is non‑negative */
        else
            temp = B - (seq_t)1;         /* n == 3 → subtract 1 */

        if (temp % M != (seq_t)0) return;
        seq_t RHS = temp / M;
        if (RHS < d) return;

        seq_t temp2 = RHS - d;
        if (temp2 >= (seq_t)2 && temp2 % (seq_t)2 == (seq_t)0) {
            ++(*count);
        }
        return;
    }

    seq_t p   = primes[pcnt - 1];
    size_t e  = exps[pcnt - 1];
    seq_t val = current;

    for (size_t i = 0; i <= e; ++i) {
        enumerate_divisors(primes, exps, pcnt - 1, val, A, M, n, count);
        val *= p;
    }
}

/* --- count representations of P_n(k)                                  */
static size_t count_representations(seq_t k, seq_t n)
{
    seq_t M = n - (seq_t)2;                 /* n‑2, always ≥1 for n≥3 */

    /* Compute A = ((n‑2)k² – (n‑4)k) with proper sign handling for n=3   */
    seq_t A;
    if (n >= (seq_t)4) {
        A = M * k * k - (n - (seq_t)4) * k;
    } else { /* n == 3 */
        A = M * k * k + ((seq_t)4 - n) * k;
    }

    /* Prime factorisation of A */
    seq_t fac_primes[64];
    size_t fac_exps[64];
    size_t fac_cnt = 0;
    factorize(A, global_primes, global_prime_count, &fac_cnt,
              fac_primes, fac_exps);

    /* Enumerate divisors and count valid representations */
    size_t reps = 0;
    enumerate_divisors(fac_primes, fac_exps, fac_cnt,
                       (seq_t)1, A, M, n, &reps);

    return reps;
}

/* ------------------------------------------------------------------ */
/*  OEIS sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;  /* undefined for n < 3 */

    init_primes();

    seq_t k = (seq_t)1;
    while (true) {
        if (count_representations(k, (seq_t)n) == (size_t)n)
            return k;
        k = k + (seq_t)1;
    }
}
