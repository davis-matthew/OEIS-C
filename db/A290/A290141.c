/*
 * A290141  Numbers n that have a record maximum (> n) in their aliquot
 *          sequence.
 * https://oeis.org/A290141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iteratively compute the maximum of each aliquot sequence
   (using trial division) and record numbers that set a new record
   maximum > n. a(n) is the n‑th such record holder. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* ---------- Prime sieve (up to a fixed limit) ---------- */
static idx_t *primes = NULL;
static size_t prime_count = 0;

static void init_primes(void)
{
    if (primes) return;                     /* already initialized */

    /* limit big enough to factor numbers up to ~2·10⁶ – sufficient for the
       task with n ≤ 10 000 and most test cases; if a larger range is needed,
       increase LIMIT. */
    const idx_t LIMIT = 2000000;

    bool *is_prime = (bool *)calloc((size_t)(LIMIT + 1), sizeof(bool));
    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++prime_count;

    primes = (idx_t *)malloc(prime_count * sizeof(idx_t));
    size_t p = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[p++] = i;

    free(is_prime);
}

/* ---------- Cache for maximum aliquot values ---------- */
static seq_t *max_cache = NULL;        /* maximum value for n, if known */
static unsigned char *max_used = NULL; /* flag: data present */
static size_t cache_cap = 0;

static void cache_ensure(idx_t need)
{
    if (need < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 128;
    while (new_cap <= need) new_cap <<= 1;
    max_cache = (seq_t *)realloc(max_cache, new_cap * sizeof(seq_t));
    max_used = (unsigned char *)realloc(max_used, new_cap * sizeof(unsigned char));
    for (size_t i = cache_cap; i < new_cap; ++i) max_used[i] = 0;
    cache_cap = new_cap;
}

/* ---------- Sum of proper divisors (trial division) ---------- */
static seq_t sum_of_proper_divisors(uint64_t n)
{
    if (n <= 1) return 0;   /* σ(n) – n = 0 for n = 0,1 */

    uint64_t temp = n;
    seq_t sigma = 1;        /* product over prime powers */

    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            seq_t term = 1;   /* (p⁰ + p¹ + … + pᵉ) */
            seq_t p_pow = 1;
            while (temp % p == 0) {
                temp /= p;
                p_pow *= p;
                term += p_pow;
            }
            sigma *= term;
        }
    }

    if (temp > 1)                          /* remaining prime factor */
        sigma *= ((seq_t)temp + 1);

    return sigma - (seq_t)n;
}

/* ---------- Compute maximum of the aliquot sequence of n ---------- */
static seq_t max_alias_of(uint64_t start)
{
    cache_ensure((idx_t)start);

    if ((idx_t)start < cache_cap && max_used[start]) return max_cache[start];

    /* Track visited numbers so we can cache later and terminate cycles. */
    uint64_t *path = NULL;
    size_t plen = 0, pcap = 0;

    seq_t max_val = (seq_t)start;
    uint64_t cur   = start;

    while (true) {
        if ((idx_t)cur < cache_cap && max_used[cur]) {
            seq_t cached = max_cache[cur];
            if (cached > max_val) max_val = cached;
            break;
        }

        /* store the current number on the path */
        if (plen == pcap) {
            size_t newcap = pcap ? pcap << 1 : 8;
            path = (uint64_t *)realloc(path, newcap * sizeof(uint64_t));
            pcap = newcap;
        }
        path[plen++] = cur;
        seq_t cur_seq = (seq_t)cur;
        if (cur_seq > max_val) max_val = cur_seq;

        /* compute next value */
        seq_t next_seq = sum_of_proper_divisors(cur);
        if (next_seq > max_val) max_val = next_seq;

        /* if next exceeds 64‑bit range, stop following the chain. */
        if (next_seq > (seq_t)UINT64_MAX) break;
        uint64_t next = (uint64_t)next_seq;

        /* end of chain or cycle detection (small cycles only). */
        if (next == cur || next == 1) break;
        bool in_cycle = false;
        for (size_t k = 0; k < plen; ++k) {
            if (path[k] == next) { in_cycle = true; break; }
        }
        if (in_cycle) break;

        cur = next;
    }

    /* Cache the result for all numbers on the path. */
    for (size_t i = 0; i < plen; ++i) {
        uint64_t x = path[i];
        if ((idx_t)x < cache_cap && !max_used[x]) {
            max_cache[x] = max_val;
            max_used[x] = 1;
        }
    }

    free(path);
    return max_val;
}

/* ---------- Global state for the record sequence ---------- */
static idx_t   *records   = NULL;   /* stored record holder numbers */
static size_t  record_cap = 0;
static size_t  record_count = 0;
static idx_t  last_checked  = 1;    /* last n processed */
static seq_t  best_max      = 0;    /* maximum seen so far */

/* ---------- Public function as required ---------- */
seq_t a(idx_t n)
{
    init_primes();                         /* ensure primes are ready */

    if (record_cap == 0) {
        record_cap = 16;
        records = (idx_t *)malloc(record_cap * sizeof(idx_t));
    }

    /* Enumerate until we have at least n record holders. */
    while (record_count < n) {
        ++last_checked;                   /* check next integer n */
        seq_t m = max_alias_of(last_checked);
        if (m > (seq_t)last_checked && m > best_max) {
            /* new record holder */
            if (record_count >= record_cap) {
                record_cap <<= 1;
                records = (idx_t *)realloc(records, record_cap * sizeof(idx_t));
            }
            records[record_count++] = last_checked;
            best_max = m;
        }
    }

    return (seq_t)records[n - 1];
}
