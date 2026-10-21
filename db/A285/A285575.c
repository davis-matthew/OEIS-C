/*
 * A285575  Lexicographically earliest sequence of distinct positive terms
 *          such that the product of two consecutive terms is divisible by p^2
 *          for at least two distinct primes p.
 * https://oeis.org/A285575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = lexicographically earliest permutation of distinct
 *            positive integers such that for every i ≥ 1, a(i) * a(i+1) is
 *            divisible by p² for at least two distinct primes p. The sequence
 *            is constructed greedily: a(1)=1; for n>1, a(n) is the smallest
 *            unused positive integer satisfying the condition...
 * Range:     u8 n <= 172, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static seq_t *seq = NULL;          /* stored sequence values          */
static size_t seq_len = 0;         /* number of terms computed       */
static size_t seq_cap = 0;         /* capacity of seq array          */

static uint64_t *used_bits = NULL; /* bitset tracking used numbers   */
static size_t used_bits_cap = 0;    /* number of 64‑bit blocks used   */

static uint32_t *spf = NULL;           /* smallest prime factor table   */
static uint32_t *primes_arr = NULL;    /* list of primes produced by sieve */
static idx_t spf_limit = 0;            /* sieve limit                    */
static size_t primes_cnt = 0;          /* number of primes found         */

/*------------------------------------------------------------*/
/*  Ensure the bitset can address number n                     */
static void
ensure_used_bits(idx_t n)
{
    idx_t idx = n >> 6;                      /* block number */
    while (used_bits_cap <= idx) {
        size_t new_cap = used_bits_cap ? used_bits_cap << 1 : 1;
        if (new_cap <= idx) new_cap = idx + 1;
        uint64_t *new_bits = (uint64_t*)calloc(new_cap, sizeof(uint64_t));
        if (!new_bits) abort();
        if (used_bits) {
            memcpy(new_bits, used_bits,
                   used_bits_cap * sizeof(uint64_t));
            free(used_bits);
        }
        used_bits = new_bits;
        used_bits_cap = new_cap;
    }
}

/*------------------------------------------------------------*/
/*  Mark number n as used                                      */
static void
mark_used(idx_t n)
{
    ensure_used_bits(n);
    used_bits[n >> 6] |= (uint64_t)1 << (n & 63);
}

/*------------------------------------------------------------*/
/*  Test whether number n is already used                     */
static bool
is_used(idx_t n)
{
    ensure_used_bits(n);
    return (used_bits[n >> 6] >> (n & 63)) & 1ULL;
}

/*------------------------------------------------------------*/
/*  Sieve of Eratosthenes: linear sieve to populate spf and primes_arr  */
static void
init_sieve(idx_t limit)
{
    if (spf) return;                /* already initialised */

    spf_limit = limit;
    spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    if (!spf) abort();

    size_t est = limit / 10 + 1;     /* rough estimate of prime count   */
    primes_arr = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!primes_arr) abort();
    primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!spf[i]) {
            spf[i] = (uint32_t)i;
            if (primes_cnt >= est) {
                est = est * 2 + 10;
                uint32_t *tmp = (uint32_t*)realloc(primes_arr,
                                                  est * sizeof(uint32_t));
                if (!tmp) abort();
                primes_arr = tmp;
            }
            primes_arr[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt &&
             (idx_t)primes_arr[j] <= spf[i] &&
             i * (idx_t)primes_arr[j] <= limit; ++j)
        {
            spf[i * (idx_t)primes_arr[j]] = (uint32_t)primes_arr[j];
        }
    }
}

/*------------------------------------------------------------*/
/*  Factor n into primes and exponents (output to arrays)    */
static void
factor_number(idx_t n, uint32_t primes_out[64], uint32_t exps_out[64],
              size_t *len)
{
    *len = 0;
    while (n > 1) {
        uint32_t p = 0;
        if (n <= spf_limit && spf[(size_t)n] != 0) {
            p = spf[(size_t)n];
        } else {
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint32_t pr = primes_arr[i];
                if ((idx_t)pr * pr > n) break;
                if (n % pr == 0) { p = pr; break; }
            }
            if (p == 0) p = (uint32_t)n;
        }
        uint32_t cnt = 0;
        while (n % p == 0) { n /= p; ++cnt; }
        primes_out[*len] = p;
        exps_out[*len]   = cnt;
        ++(*len);
    }
}

/*------------------------------------------------------------*/
/*  Test if candidate cand satisfies the property with cur    */
static bool
candidate_ok(idx_t cur, const uint32_t curp[64], const uint32_t cure[64],
             size_t cur_len, int count0, idx_t cand)
{
    int cnt = count0;   /* primes in cur with exponent ≥2 already counted */

    /* primes in both with cur exponent 1 -> cand must supply exponent ≥1 */
    for (size_t i = 0; i < cur_len; ++i) {
        if (cure[i] == 1 && cand % curp[i] == 0) ++cnt;
    }
    if (cnt >= 2) return true;

    /* factor candidate */
    uint32_t candp[64], cand_e[64];
    size_t cand_len;
    factor_number(cand, candp, cand_e, &cand_len);

    for (size_t i = 0; i < cand_len; ++i) {
        uint32_t p = candp[i];
        bool in_cur = false;
        for (size_t j = 0; j < cur_len; ++j)
            if (curp[j] == p) { in_cur = true; break; }
        if (!in_cur && cand_e[i] >= 2) {
            ++cnt;
            if (cnt >= 2) return true;
        }
    }
    return cnt >= 2;
}

/*------------------------------------------------------------*/
/*  Main required function                                    */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;           /* off‑by‑one safety */

    /* lazy initialisation */
    if (!seq) {
        seq_cap = 128;
        seq = (seq_t*)calloc(seq_cap, sizeof(seq_t));
        seq_len = 1;
        seq[0] = (seq_t)1;         /* a(1) = 1 */

        used_bits_cap = 0;
        used_bits = NULL;
        mark_used(1);              /* mark 1 as used */

        init_sieve(2000000);       /* sieve up to 2·10^6 */
    }

    while (seq_len < n) {
        idx_t cur = (idx_t)seq[seq_len - 1];

        /* factor current term */
        uint32_t curp[64], cure[64];
        size_t cur_len;
        factor_number(cur, curp, cure, &cur_len);

        int count0 = 0;               /* primes with exponent ≥2 in cur */
        for (size_t i = 0; i < cur_len; ++i)
            if (cure[i] >= 2) ++count0;

        /* search for the smallest unused candidate */
        idx_t cand = 1;
        for (;;) {
            if (!is_used(cand) &&
                candidate_ok(cur, curp, cure, cur_len, count0, cand)) {
                /* expand storage if needed */
                if (seq_len >= seq_cap) {
                    seq_cap <<= 1;
                    seq_t *tmp = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    seq = tmp;
                }
                seq[seq_len++] = (seq_t)cand;
                mark_used(cand);
                break;
            }
            ++cand;
        }
    }
    return seq[n - 1];
}
