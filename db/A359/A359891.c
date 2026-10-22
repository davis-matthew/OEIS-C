/*
 * A359891  Members of A026424 (numbers with an odd number of prime factors)
 *          whose prime indices have the same mean as median.
 * https://oeis.org/A359891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 6811, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A359891: intersection of A026424 (odd ω(n)) and A359889
 * (numbers whose prime‑index multiset has mean = median).
 * Computed by incremental sieving and testing each integer. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- global state ---------- */
static uint32_t *spf = NULL;           /* smallest prime factor table */
static uint32_t *primes = NULL;        /* list of primes */
static size_t primes_cnt = 0;          /* number of primes */
static idx_t sieve_limit = 0;          /* largest number for which spf is valid */

static seq_t *seq_arr = NULL;          /* cached sequence terms */
static size_t seq_cap = 0;
static idx_t seq_len = 0;
static idx_t next_num = 2;             /* next integer to test */

/* ---------- helper: build sieve up to 'limit' ---------- */
static void build_sieve(idx_t limit) {
    if (limit <= sieve_limit) return;
    if (spf) free(spf);
    if (primes) free(primes);
    spf = (uint32_t *)calloc((size_t)limit + 1, sizeof(uint32_t));
    if (!spf) exit(1);
    size_t approx = (size_t)limit / 10 + 100;
    primes = (uint32_t *)malloc(approx * sizeof(uint32_t));
    if (!primes) exit(1);
    size_t pn = 0;
    for (uint64_t i = 2; i <= (uint64_t)limit; ++i) {
        if (spf[(size_t)i] == 0) {
            spf[(size_t)i] = (uint32_t)i;
            if (pn == approx) {
                approx *= 2;
                primes = (uint32_t *)realloc(primes, approx * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[pn++] = (uint32_t)i;
        }
        for (size_t j = 0; j < pn && primes[j] <= spf[(size_t)i] &&
              (uint64_t)i * primes[j] <= (uint64_t)limit; ++j) {
            spf[(size_t)i * primes[j]] = primes[j];
        }
    }
    primes_cnt = pn;
    sieve_limit = limit;
}

/* ---------- helper: binary search for prime index (1‑based) ---------- */
static size_t get_prime_idx(uint32_t p) {
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return mid + 1;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0; /* should never happen for a valid prime */
}

/* ---------- helper: test if n satisfies the two conditions ---------- */
static bool candidate_ok(uint64_t n) {
    if (n < 2) return false;

    /* factorize n storing indices and exponents */
    uint32_t js[32];
    uint32_t es[32];
    size_t k = 0;
    size_t L = 0;
    uint64_t x = n;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        uint32_t e = 0;
        while (x % p == 0) { x /= p; ++e; }
        uint32_t idx = (uint32_t)get_prime_idx(p);
        js[k] = idx;
        es[k] = e;
        L += e;
        ++k;
    }
    if ((L & 1u) == 0) return false;   /* need odd omega */

    /* sum of weighted indices */
    uint64_t sum = 0;
    for (size_t i = 0; i < k; ++i)
        sum += (uint64_t)es[i] * (uint64_t)js[i];

    /* median index */
    size_t target = (L + 1) >> 1;          /* position of median (1‑based) */
    size_t cum = 0;
    size_t median = 0;
    for (size_t i = 0; i < k; ++i) {
        cum += es[i];
        if (cum >= target) {
            median = js[i];
            break;
        }
    }
    if (median == 0) return false;

    return sum == (uint64_t)median * L;
}

/* ---------- helper: grow sequence array ---------- */
static void grow_seq(size_t needed) {
    if (seq_cap >= needed) return;
    seq_cap = seq_cap * 2;
    if (seq_cap < needed) seq_cap = needed;
    seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
    if (!seq_arr) exit(1);
}

/* ---------- main function: a(n) ---------- */
seq_t a(idx_t n) {
    if (seq_arr == NULL) {
        seq_cap = 32;
        seq_arr = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_arr) exit(1);
    }
    /* Generate terms until we have at least n of them */
    while (seq_len < n) {
        if ((idx_t)next_num > sieve_limit)
            build_sieve((idx_t)(next_num * 2 + 1000));
        if (candidate_ok((uint64_t)next_num)) {
            grow_seq((size_t)(seq_len + 1));
            seq_arr[seq_len++] = (seq_t)next_num;
        }
        ++next_num;
    }
    return seq_arr[n - 1];
}
