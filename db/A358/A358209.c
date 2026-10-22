/*
 * A358209  a(1) = 1; a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number not occurring earlier that shares a factor with
 *          A024916(n-1) = Sum_{k=1..n-1} sigma(k).
 * https://oeis.org/A358209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2) = 2; for n > 2, * a(n) is the smallest positive
 *            integer not yet used that shares * a non‑trivial factor with
 *            S(n‑1) = ∑_{k=1}^{n‑1} σ(k)
 * Range:     u8 n <= 18, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- static data -------------------------------------------- */
static seq_t *sequence = NULL;   /* a(1) stored at index 0        */
static size_t seq_len   = 0;     /* number of terms stored         */
static size_t seq_cap   = 0;     /* capacity of sequence array     */

static seq_t *used      = NULL;  /* list of all used numbers       */
static size_t used_len  = 0;
static size_t used_cap  = 0;

static seq_t cumulative_sigma = 0;   /* ∑_{k=1}^{m-1} σ(k) */

/* ---------- prime sieve -------------------------------------------- */
#define PRIME_LIMIT 200000
static uint64_t primes[PRIME_LIMIT];
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (prime_cnt) return;
    bool *is_prime = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) abort();
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    for (uint64_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;
    free(is_prime);
}

/* ---------- dynamic array helpers --------------------------------- */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = (seq_cap > 0) ? seq_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(sequence, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    sequence = tmp;
    seq_cap   = new_cap;
}

static void ensure_used_capacity(void)
{
    if (used_cap > used_len) return;
    size_t new_cap = (used_cap > 0) ? used_cap << 1 : 16;
    seq_t *tmp = (seq_t*)realloc(used, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    used = tmp;
    used_cap = new_cap;
}

static bool used_contains(seq_t val)   /* must precede used_add */
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == val) return true;
    return false;
}

static void used_add(seq_t val)
{
    if (used_contains(val)) return;    /* should never happen */
    ensure_used_capacity();
    used[used_len++] = val;
}

/* ---------- sigma(k) using prime factorisation ------------------- */
static seq_t sigma_ui(uint64_t n)
{
    seq_t result = 1;
    uint64_t temp  = n;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            seq_t term = 1;
            seq_t pow  = 1;
            while (temp % p == 0) {
                temp /= p;
                pow  *= (seq_t)p;
                term += pow;
            }
            result *= term;
        }
    }
    if (temp > 1) result *= (seq_t)(temp + 1);
    return result;
}

/* ---------- build sequence up to required index ----------------- */
static void compute_up_to(idx_t n)
{
    if (n <= seq_len) return;
    init_primes();
    ensure_seq_capacity(n);   /* need indices 0 .. n-1 */

    for (idx_t m = seq_len + 1; m <= n; ++m) {
        seq_t val;
        if (m == 1) {
            val = 1;
        } else if (m == 2) {
            val = 2;
        } else {
            /* factor S = cumulative_sigma = ∑_{k=1}^{m-1} σ(k) */
            uint64_t S = (uint64_t)cumulative_sigma;
            uint64_t temp = S;
            uint64_t fac[64];
            size_t fac_cnt = 0;
            for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
                uint64_t p = primes[i];
                if (temp % p == 0) {
                    fac[fac_cnt++] = p;
                    while (temp % p == 0) temp /= p;
                }
            }
            if (temp > 1 && fac_cnt < 64) fac[fac_cnt++] = temp;

            /* find smallest unused multiple of any prime factor */
            seq_t best = 0;
            for (size_t fi = 0; fi < fac_cnt; ++fi) {
                seq_t p = (seq_t)fac[fi];
                seq_t cand = p;
                while (used_contains(cand)) cand += p;
                if (!best || cand < best) best = cand;
            }
            val = best;
        }

        /* store the value */
        sequence[seq_len] = val;
        ++seq_len;
        used_add(val);

        /* update cumulative sigma for next step */
        seq_t sigma_val = sigma_ui((uint64_t)m);
        cumulative_sigma += sigma_val;
    }
}

/* ---------- public interface -------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* defensive; OEIS offset is 1 */
    compute_up_to(n);
    return sequence[n - 1];          /* a(1) stored at index 0 */
}
