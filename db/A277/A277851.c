/*
 * A277851  Numbers not occurring in A075771 (= q(n) + r(n), with n^2 =
 *          prime(n)*q(n) + r(n), 0 <= r(n) < prime(n)).
 * https://oeis.org/A277851
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

/*  This function computes OEIS sequence A277851.
 *  It generates enough values of the related sequence A075771
 *  (q(n)+r(n) with n² = prime(n)*q(n)+r(n)), marks them, and
 *  returns the n‑th integer that was never seen.  The algorithm
 *  requires only O(N) memory for N≈2·10⁵ and is fast enough for
 *  n up to about 10 000.  All allocations are cast so that the
 *  code compiles both under C11 and C++17.  */

#include <stdint.h>   /* uint64_t, uint32_t */
#include <stdlib.h>   /* malloc, calloc, realloc, abort */
#include <stdbool.h>  /* bool */
#include <stddef.h>   /* size_t */

/* ---------- static state ---------- */
static bool initialized = false;
static uint32_t *primes = NULL;          /* up to PRECOMP_N primes          */
static size_t primes_cnt = 0;
static unsigned char *seen_bits = NULL;  /* bitset of numbers seen in A075771 */
static size_t seen_bytes_cap = 0;        /* capacity of seen_bits in bytes   */
static seq_t *missing = NULL;            /* array of missing numbers         */
static size_t missing_cap = 0, missing_cnt = 0;
static size_t missing_next_start = 1;    /* next integer to test for missing */

/* ---------- helper macros ---------- */
#define REALLOC_ARRAY(ptr, cap, type, new_cap)                               \
    do {                                                                     \
        if ((new_cap) > (cap)) {                                            \
            size_t tmp_cap = (cap) * 2;                                      \
            if (tmp_cap < (new_cap)) tmp_cap = (new_cap);                   \
            type *tmp = (type *)realloc((ptr), tmp_cap * sizeof(type));    \
            if (!tmp) abort();                                              \
            (ptr) = tmp;                                                     \
            (cap) = tmp_cap;                                                \
        }                                                                    \
    } while (0)

/* ---------- bitset helpers ---------- */
static inline void enlarge_seen_bits(size_t idx)
{
    size_t needed_bytes = (idx >> 3) + 1;
    if (needed_bytes > seen_bytes_cap) {
        size_t new_cap = seen_bytes_cap ? seen_bytes_cap * 2 : 64;
        if (new_cap < needed_bytes) new_cap = needed_bytes;
        unsigned char *tmp = (unsigned char *)realloc(seen_bits, new_cap);
        if (!tmp) abort();
        for (size_t i = seen_bytes_cap; i < new_cap; ++i) tmp[i] = 0;
        seen_bits = tmp;
        seen_bytes_cap = new_cap;
    }
}
static inline void set_seen(size_t v)
{
    enlarge_seen_bits(v);
    seen_bits[v >> 3] |= (1u << (v & 7));
}
static inline bool is_seen(size_t v)
{
    size_t byte = v >> 3;
    if (byte >= seen_bytes_cap) return false;
    return (seen_bits[byte] & (1u << (v & 7))) != 0;
}

/* ---------- sieve of Eratosthenes ---------- */
static void sieve_first_n_primes(idx_t n_needed,
                                 uint32_t **out, size_t *cnt_out)
{
    /* Rough upper bound for the n-th prime: n*(log n + log log n) < 2n log n.
     * For n<=200000 this is <4.0e6.  We add a margin. */
    idx_t limit = n_needed * 20 + 100;
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp) abort();

    uint32_t *buf = (uint32_t *)malloc(n_needed * sizeof(uint32_t));
    if (!buf) abort();

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit && cnt < n_needed; ++i) {
        if (!is_comp[i]) {
            buf[cnt++] = (uint32_t)i;
            if ((uint64_t)i * (uint64_t)i <= (uint64_t)limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    if (cnt < n_needed) abort();   /* defensive: should never happen */

    *out = buf;
    *cnt_out = cnt;
}

/* ---------- precomputation ---------- */
static void init(void)
{
    if (initialized) return;
    /* Precompute enough terms of A075771 to cover thousands of missing
     * numbers for n up to about 10 000. */
    const idx_t PRECOMP_N = 200000;

    /* generate primes */
    sieve_first_n_primes(PRECOMP_N, &primes, &primes_cnt);

    /* init seen bitset */
    seen_bytes_cap = 64;
    seen_bits = (unsigned char *)calloc(seen_bytes_cap, sizeof(unsigned char));
    if (!seen_bits) abort();

    /* compute A075771 values and mark them as seen */
    for (idx_t idx = 1; idx <= PRECOMP_N; ++idx) {
        uint32_t p = primes[idx - 1];
        unsigned __int128 n2 = (unsigned __int128)idx * (unsigned __int128)idx;
        uint64_t q = (uint64_t)(n2 / p);
        uint64_t r = (uint64_t)(n2 % p);
        uint64_t val = q + r;               /* val fits in 64‑bit for our limits */
        set_seen((size_t)val);
    }

    /* initialise missing array */
    missing_cap = 1024;
    missing = (seq_t *)calloc(missing_cap, sizeof(seq_t));
    if (!missing) abort();
    missing_cnt = 0;
    missing_next_start = 1;

    initialized = true;
}

/* ---------- ensure we have at least n missing numbers ---------- */
static void ensure_missing_at_least(idx_t want)
{
    if (missing_cnt >= want) return;

    while (missing_cnt < want) {
        if (missing_cnt == missing_cap) {
            missing_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(missing, missing_cap * sizeof(seq_t));
            if (!tmp) abort();
            missing = tmp;
        }
        if (!is_seen(missing_next_start)) {
            missing[missing_cnt++] = (seq_t)missing_next_start;
        }
        ++missing_next_start;
    }
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) abort();          /* offset 1 */
    init();
    ensure_missing_at_least(n);
    return missing[n - 1];
}
