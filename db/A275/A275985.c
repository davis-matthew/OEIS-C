/*
 * A275985  Least k such that n divides phi(k!) (k > 0).
 * https://oeis.org/A275985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least k>0 such that n divides φ(k!)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* ---------- global data for sieve (cached across calls) ---------- */
static size_t prime_cnt = 0;          /* number of primes found   */
static size_t prime_cap = 0;          /* capacity of prime_list   */
static uint64_t *prime_list = NULL;   /* list of primes up to current limit */
static idx_t *spf = NULL;             /* smallest prime factor array       */
static int *prime_idx = NULL;         /* map prime → index in prime_list   */
static idx_t max_limit = 0;           /* largest number already sieved */

/* make sure prime_list has at least 'needed' slots */
static void ensure_prime_capacity(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t newcap = prime_cap * 2 + 16;
    if (newcap < needed) newcap = needed;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, newcap * sizeof(uint64_t));
    if (!tmp) exit(1);
    prime_list = tmp;
    prime_cap = newcap;
}

/* simple sieve with spf and prime index up to 'limit' */
static void build_sieve(idx_t limit)
{
    if (limit <= max_limit) return;      /* already enough */

    /* free previous arrays */
    free(prime_list);
    free(spf);
    free(prime_idx);

    prime_list = NULL; prime_cap = 0;
    spf  = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    prime_idx = (int *)malloc((size_t)(limit + 1) * sizeof(int));
    if (!spf || !prime_idx) exit(1);

    for (idx_t i = 0; i <= limit; ++i) {
        spf[i] = i;
        prime_idx[i] = -1;
    }

    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == i) {                           /* i is prime */
            ensure_prime_capacity(prime_cnt + 1);
            prime_list[prime_cnt] = i;
            prime_idx[i] = (int)prime_cnt;
            ++prime_cnt;
            if (i > limit / i) continue;             /* would overflow */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    max_limit = limit;
}

/* multiplicative modular reduction, works for integer and bigint seq_t */
static inline seq_t mul_mod_seq(seq_t a, seq_t b, seq_t mod)
{
    return (seq_t)((a * b) % mod);
}

/* modular exponentiation for small exponents */
static seq_t pow_mod_small(seq_t base, size_t exp, seq_t mod)
{
    seq_t res = 1 % mod;
    while (exp) {
        if (exp & 1) res = mul_mod_seq(res, base, mod);
        base = mul_mod_seq(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* --------- main OEIS sequence function ---------- */
seq_t a(idx_t n)
{
    if (n <= 1) return 1;

    /* Upper bound for k is 2*n. */
    idx_t limit_k = n * 2;
    build_sieve(limit_k);                 /* primes up to k */

    /* allocate exponent array for primes, initial zero. */
    size_t *exp = (size_t *)calloc(prime_cnt, sizeof(size_t));
    if (!exp) exit(1);

    seq_t mod_n = (seq_t)n;
    seq_t phi = 1 % mod_n;                /* φ(k!) modulo n */

    for (idx_t k = 1; k <= limit_k; ++k) {
        /* factorize k using spf */
        idx_t temp = k;
        while (temp > 1) {
            uint64_t p = spf[temp];
            size_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            size_t idx = (size_t)prime_idx[p];
            if (exp[idx] == 0) {          /* first appearance of p */
                /* multiply by (p-1) */
                phi = mul_mod_seq(phi, (seq_t)(p - 1), mod_n);
                /* multiply by p^{e-1} if e > 1 */
                if (e > 1) {
                    seq_t pe = pow_mod_small((seq_t)p, e - 1, mod_n);
                    phi = mul_mod_seq(phi, pe, mod_n);
                }
                exp[idx] = e;
            } else {
                /* existing exponent exp[idx]; new total = exp[idx] + e */
                seq_t pe = pow_mod_small((seq_t)p, e, mod_n);
                phi = mul_mod_seq(phi, pe, mod_n);
                exp[idx] += e;
            }
        }

        if (phi == 0) { /* n divides φ(k!) */
            free(exp);
            return k;
        }
    }

    free(exp);
    return limit_k;   /* should never happen (a(n) ≤ 2n) */
}
