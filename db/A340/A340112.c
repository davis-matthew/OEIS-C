/*
 * A340112  Prime-acronym numbers: sums of primes which also equal the
 *          concatenation of the initial digits of these primes.
 * https://oeis.org/A340112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A340112 – Prime‑acronym numbers.
 * a(n) is the nth integer which can be written as the sum of a multiset
 * of primes, where that multiset is also obtained by taking the initial
 * decimal digit of each prime and concatenating them to form the same
 * integer.  The implementation caches primes per leading digit and tests
 * integers lazily, memoising found terms.  It works for any seq_t/idx_t
 * width defined by the harness.  */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>          /* for sqrt() */

/* Helper structures ----------------------------------------------------- */

typedef struct {
    uint64_t *data;
    size_t size;
    size_t cap;
} DigitList;

static DigitList digit_lists[10];   /* index 0 unused – leading digits 1‑9 */
static seq_t min_prime[10] = {0};   /* smallest prime for each leading digit */

/* Prime sieve handling --------------------------------------------------- */

static idx_t prime_limit = 0;       /* current upper bound of precomputed primes */

static void clear_digit_lists(void)
{
    for (int d = 0; d < 10; ++d) {
        free(digit_lists[d].data);
        digit_lists[d].data = NULL;
        digit_lists[d].size = 0;
        digit_lists[d].cap  = 0;
    }
    memset(min_prime, 0, sizeof(min_prime));
}

static void append_to_list(DigitList *lst, uint64_t val)
{
    if (lst->cap == 0) {
        lst->cap = 128;
        lst->data = (uint64_t *)malloc(lst->cap * sizeof(uint64_t));
    } else if (lst->size == lst->cap) {
        lst->cap <<= 1;
        lst->data = (uint64_t *)realloc(lst->data, lst->cap * sizeof(uint64_t));
    }
    lst->data[lst->size++] = val;
}

static inline uint8_t leading_digit_uint64(uint64_t v)
{
    while (v >= 10) v /= 10;
    return (uint8_t)v;
}

static void re_sieve(idx_t limit)
{
    clear_digit_lists();
    bool *composite = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!composite) return;          /* unlikely in test harness */

    idx_t sqrt_lim = (idx_t)floor(sqrt((double)limit));
    for (idx_t i = 2; i <= sqrt_lim; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                composite[j] = true;
        }
    }
    for (idx_t i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            uint8_t d = leading_digit_uint64((uint64_t)i);
            append_to_list(&digit_lists[d], (uint64_t)i);
            if (min_prime[d] == 0 || i < min_prime[d])
                min_prime[d] = (seq_t)i;
        }
    }
    free(composite);
    prime_limit = limit;
}

static void ensure_prime_limit(idx_t needed)
{
    if (prime_limit >= needed) return;
    idx_t new_limit = prime_limit ? prime_limit << 1 : 64;
    while (new_limit < needed) new_limit <<= 1;
    re_sieve(new_limit);
}

/* Utility: integer upper bound in an array [0..sz) of uint64_t                */
static inline size_t upper_bound_uint64(const uint64_t *arr, size_t sz, seq_t val)
{
    size_t lo = 0, hi = sz;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((seq_t)arr[mid] <= val) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

/* Check whether n is a prime‑acronym number -------------------------------- */
static bool dfs_check(size_t idx, seq_t cur_sum, const uint8_t *digits,
                      size_t k, seq_t target, const seq_t *suf_min)
{
    if (cur_sum > target) return false;
    if (cur_sum + suf_min[idx] > target) return false;
    if (idx == k) return cur_sum == target;

    uint8_t d = digits[idx];
    const DigitList *lst = &digit_lists[d];
    if (lst->size == 0) return false;

    seq_t remaining = target - cur_sum;
    size_t max_i = upper_bound_uint64(lst->data, lst->size, remaining);
    for (size_t i = 0; i < max_i; ++i) {
        if (dfs_check(idx + 1, cur_sum + (seq_t)lst->data[i],
                      digits, k, target, suf_min))
            return true;
    }
    return false;
}

static bool is_prime_acronym(idx_t n)
{
    if (n < 2) return false;

    /* Extract decimal digits of n (most significant first) */
    uint8_t digits[30];
    size_t k = 0;
    uint64_t tmp = (uint64_t)n;
    while (tmp > 0) {
        digits[k++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }
    for (size_t i = 0; i < k / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[k - 1 - i];
        digits[k - 1 - i] = t;
    }

    /* Quick rejection if any digit is 0 or has no primes */
    for (size_t i = 0; i < k; ++i) {
        if (digits[i] == 0) return false;
        if (min_prime[digits[i]] == 0) return false;
    }

    /* Compute suffix minimal sum to aid pruning */
    seq_t suf_min[31];
    suf_min[k] = 0;
    for (size_t i = k; i-- > 0;) {
        seq_t add = min_prime[digits[i]];
        suf_min[i] = suf_min[i + 1] + add;
        if (suf_min[i] > (seq_t)n) return false;
    }

    return dfs_check(0, 0, digits, k, (seq_t)n, suf_min);
}

/* Sequence generation memoisation ---------------------------------------- */

#define MAX_TERMS 20000

static seq_t term_cache[MAX_TERMS];
static idx_t term_count = 0;
static idx_t next_candidate = 2;   /* next integer to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but harmless */
    if (term_count >= n) return term_cache[n - 1];

    /* initialise primes up to a modest starting limit */
    if (prime_limit == 0) re_sieve(1024);

    while (term_count < n) {
        ensure_prime_limit(next_candidate);
        if (is_prime_acronym(next_candidate)) {
            if (term_count < MAX_TERMS) term_cache[term_count++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return term_cache[n - 1];
}
