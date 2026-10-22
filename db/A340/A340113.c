/*
 * A340113  Acronym primes: primes which are the sums of primes and also equal
 *          the concatenation of the initial digits of these primes.
 * https://oeis.org/A340113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime that is a sum of primes whose * concatenation
 *            of their first digits equals the prime itself. * The code
 *            generates primes sequentially, stores primes * grouped by their
 *            first digit, and for each new prime checks * whether it can be
 *            decomposed into such a sum using DFS. * A cache h...
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

typedef struct {
    seq_t *data;    /* list of primes with this first digit, sorted ascending */
    size_t len;     /* number of elements stored */
    size_t cap;     /* capacity of the array */
} DigitList;

static DigitList digit_lists[10];
static seq_t min_prime_digit[10] = {0};   /* minimal prime having this first digit */
static seq_t *cache   = NULL;            /* cache of found acronym primes */
static size_t cache_len = 0, cache_cap = 0;
static bool initialized = false;
static seq_t last_prime   = 0;           /* last prime generated, for next_prime() */

/* ------------------------------------------------------------------ */
/* Utilities for prime generation ----------------------------------- */
/* ------------------------------------------------------------------ */

static inline seq_t next_prime(seq_t current) {
    seq_t cand = current + 1;
    if (cand < 2) cand = 2;
    if (cand > 2 && (cand & 1) == 0) cand++;              /* make odd */
    for (;;) {
        if (is_prime((idx_t)cand)) return cand;
        cand += 2;
    }
}

/* ------------------------------------------------------------------ */
/* Digit handling --------------------------------------------------- */
/* ------------------------------------------------------------------ */

static inline uint8_t first_digit(seq_t n) {
    while (n >= 10) n /= 10;
    return (uint8_t)n;
}

/* Convert number into most‑significant‑first digit array.
 * Returns number of digits.                                    */
static int num_digits(seq_t n, uint8_t *buf, int maxlen) {
    int i = 0;
    do {
        if (i >= maxlen) return -1;
        buf[i++] = (uint8_t)(n % 10);
        n /= 10;
    } while (n);
    /* reverse */
    for (int j = 0; j < i/2; ++j) {
        uint8_t t = buf[j];
        buf[j] = buf[i-1-j];
        buf[i-1-j] = t;
    }
    return i;
}

/* ------------------------------------------------------------------ */
/* Storage of primes per first digit -------------------------------- */
/* ------------------------------------------------------------------ */

static void add_prime_to_digit_list(seq_t p) {
    uint8_t d = first_digit(p);
    DigitList *dl = &digit_lists[d];
    if (dl->len == dl->cap) {
        size_t new_cap = dl->cap == 0 ? 8 : dl->cap * 2;
        seq_t *new_data = (seq_t*)realloc(dl->data, new_cap * sizeof(seq_t));
        dl->data = new_data;
        dl->cap = new_cap;
    }
    dl->data[dl->len++] = p;
    if (min_prime_digit[d] == 0) min_prime_digit[d] = p;
}

/* ------------------------------------------------------------------ */
/* Main DFS search -------------------------------------------------- */
/* ------------------------------------------------------------------ */

/* Recursively try to partition 'rem' into primes whose first digits
 * follow 'digits[pos..k-1]'. 'min_suf[pos]' is the minimal possible
 * sum of primes from position pos to the end.                           */
static bool dfs(const uint8_t *digits, int k, int pos, seq_t rem, const seq_t *min_suf) {
    if (pos == k) return rem == 0;
    uint8_t d = digits[pos];
    const DigitList *dl = &digit_lists[d];
    if (dl->len == 0) return false;    /* no primes with this first digit */
    seq_t min_next = min_suf[pos + 1];
    for (size_t i = 0; i < dl->len; ++i) {
        seq_t q = dl->data[i];
        if (q > rem) break;            /* array sorted ascending */
        seq_t after = rem - q;
        if (after < min_next) continue;
        if (dfs(digits, k, pos + 1, after, min_suf)) return true;
    }
    return false;
}

static bool is_acronym_prime(seq_t p) {
    if (p < 10) return true;   /* single‑digit primes trivially satisfy */

    uint8_t digits[64];
    int k = num_digits(p, digits, 64);
    if (k <= 0) return false;

    /* Quick reject if any needed first digit hasn't appeared yet */
    for (int i = 0; i < k; ++i) {
        if (min_prime_digit[digits[i]] == 0) return false;
    }

    /* Precompute suffix minimal sums for pruning */
    seq_t min_suf[65];
    min_suf[k] = 0;
    for (int i = k - 1; i >= 0; --i) {
        min_suf[i] = min_suf[i + 1] + min_prime_digit[digits[i]];
    }
    if (min_suf[0] > p) return false;

    return dfs(digits, k, 0, p, min_suf);
}

/* ------------------------------------------------------------------ */
/* Cache initialization and maintenance ----------------------------- */
/* ------------------------------------------------------------------ */

static void init_if_needed(void) {
    if (initialized) return;
    cache_cap = 1024;
    cache = (seq_t*)calloc(cache_cap, sizeof(seq_t));
    for (int d = 0; d < 10; ++d) {
        digit_lists[d].data = NULL;
        digit_lists[d].len = 0;
        digit_lists[d].cap = 0;
        min_prime_digit[d] = 0;
    }
    initialized = true;
}

static void ensure_cache(idx_t n) {
    init_if_needed();
    while (cache_len < (size_t)n) {
        last_prime = next_prime(last_prime);
        seq_t p = (seq_t)last_prime;
        add_prime_to_digit_list(p);
        if (is_acronym_prime(p)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *new_buf = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                cache = new_buf;
                cache_cap = new_cap;
            }
            cache[cache_len++] = p;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public function --------------------------------------------------- */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but safe return */
    ensure_cache(n);
    return cache[n - 1];
}
