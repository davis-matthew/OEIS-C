/*
 * A358208  a(1) = 1; a(2) = 2; a(3) = 3; for n > 3, a(n) is the smallest
 *          positive number not occurring earlier that shares a factor with
 *          Sum_{k=1..n-1} A001065(k), where A001065(k) is the sum of the
 *          proper divisors of k.
 * https://oeis.org/A358208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=3; for n>3,
   a(n) = smallest unused positive integer m such that
   gcd(m, Σ_{k=1}^{n-1} σ(k)) > 1, where σ(k) is the sum of
   proper divisors of k (A001065). */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ---------- global state --------------------------------------------- */
static seq_t *seq_arr      = NULL;  /* computed sequence values  */
static size_t seq_capacity = 0;    /* allocated seq_arr size    */
static size_t seq_size     = 0;    /* number of computed terms  */

static uint8_t *used_bits            = NULL; /* bitset: 1 = value used */
static size_t used_bits_capacity_bits = 0;    /* bit count */

/* prefix sums of σ(k) up to a fixed limit */
static uint64_t *prefix_sum      = NULL;
static size_t   prefix_capacity  = 0;  /* number of elements stored */

/* limit for precomputation of σ(k) and its prefix sums */
#define PRECOMP_LIMIT 20000U

/* ---------- helper utilities ---------------------------------------- */

/* Euclidean algorithm for 64‑bit values */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b; b = t;
    }
    return a;
}

/* ensure seq_arr can hold at least 'need' elements */
static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_capacity) return;
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!seq_arr) abort();
    seq_capacity = new_cap;
}

/* ensure used_bits can represent at least 'bits_needed' bits */
static void ensure_used_bits_capacity(size_t bits_needed)
{
    if (bits_needed <= used_bits_capacity_bits) return;
    size_t new_bytes = (bits_needed + 7) / 8;
    size_t old_bytes = (used_bits_capacity_bits + 7) / 8;
    uint8_t *new_arr = (uint8_t*)realloc(used_bits, new_bytes);
    if (!new_arr) abort();
    if (new_bytes > old_bytes)
        memset(new_arr + old_bytes, 0, new_bytes - old_bytes);
    used_bits = new_arr;
    used_bits_capacity_bits = bits_needed;
}

/* check if a value has already been used */
static bool is_used(idx_t v)
{
    if ((size_t)v >= used_bits_capacity_bits) return false;
    return (used_bits[v >> 3] >> (v & 7)) & 1;
}

/* mark a value as used */
static void mark_used(idx_t v)
{
    ensure_used_bits_capacity((size_t)v + 1);
    used_bits[v >> 3] |= (uint8_t)(1 << (v & 7));
}

/* precompute σ(k) for k <= PRECOMP_LIMIT and their prefix sums */
static void precompute_prefix(void)
{
    if (prefix_capacity > 0) return;  /* already done */

    /* allocate temporary array for σ(k) */
    uint64_t *sigma = (uint64_t*)calloc(PRECOMP_LIMIT + 1, sizeof(uint64_t));
    if (!sigma) abort();

    /* sieve to fill σ(k) for proper divisors */
    for (idx_t i = 1; i <= PRECOMP_LIMIT; ++i) {
        for (idx_t j = i + i; j <= PRECOMP_LIMIT; j += i)
            sigma[j] += (uint64_t)i;
    }

    /* allocate prefix_sum array */
    prefix_sum = (uint64_t*)malloc((PRECOMP_LIMIT + 1) * sizeof(uint64_t));
    if (!prefix_sum) abort();

    prefix_sum[0] = 0ULL;
    for (idx_t i = 1; i <= PRECOMP_LIMIT; ++i)
        prefix_sum[i] = prefix_sum[i - 1] + sigma[i];

    free(sigma);
    prefix_capacity = PRECOMP_LIMIT + 1;
}

/* compute terms up to index n (inclusive) */
static void compute_up_to(idx_t n)
{
    if (seq_arr == NULL) {
        seq_arr = (seq_t*)malloc(64 * sizeof(seq_t));
        if (!seq_arr) abort();
        seq_capacity = 64;
    }

    ensure_seq_capacity((size_t)n);
    for (idx_t idx = seq_size + 1; idx <= n; ++idx) {
        seq_t value;
        if (idx == 1 || idx == 2 || idx == 3) {
            value = (seq_t)idx;
        } else {
            /* sum of proper divisors up to idx-1 */
            uint64_t S = prefix_sum[(size_t)(idx - 1)];

            /* search for minimal unused m with gcd(m, S) > 1 */
            idx_t cand = 1;
            for (;;) {
                if (!is_used(cand) && gcd_u64((uint64_t)cand, S) > 1) {
                    value = (seq_t)cand;
                    break;
                }
                ++cand;
            }
        }
        seq_arr[idx - 1] = value;
        mark_used((idx_t)value);
    }
    seq_size = (size_t)n;
}

/* ---------- public function ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, safety return */

    /* one‑time initialization */
    static int init_flag = 0;
    if (!init_flag) {
        precompute_prefix();
        init_flag = 1;
    }

    if ((size_t)n > seq_size) compute_up_to(n);
    return seq_arr[n - 1];
}
