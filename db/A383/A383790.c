/*
 * A383790  Prime numbers in order of occurrence as substrings in the
 *          concatenation of natural numbers 123456789101112....
 * https://oeis.org/A383790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 17, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A383790: primes in order of first appearance as substrings of
   the concatenation 123456789101112...  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* ---------- local helpers ---------- */

/* deterministic Miller‑Rabin for 64‑bit integers (works for all unsigned
   64‑bit numbers).  The routine is very small and fast. */
static uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static uint64_t mod_pow64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d) {
        if (d & 1) result = mod_mul64(result, a, mod);
        a = mod_mul64(a, a, mod);
        d >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases that are enough for 64‑bit deterministic check */
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (!a) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* select primality test appropriate for the current seq_t type */
static bool is_prime_seq(seq_t v)
{
    /* if seq_t fits in an unsigned 64‑bit integer, use the fast test,
       otherwise defer to the big‑integer helper. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        return is_prime_uint64((uint64_t)v);
    } else {
        return is_prime_big(v);
    }
}

/* compute at most how many decimal digits can safely be stored in seq_t
   (for fixed‑size unsigned types); for arbitrary big types we use a
   generous bound 200. */
static size_t max_seq_digits = 0;
static void compute_max_seq_digits(void)
{
    if (max_seq_digits) return;
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned long long val = ~0ULL;
        size_t count = 0;
        while (val) { val /= 10; ++count; }
        max_seq_digits = count;
    } else {
        max_seq_digits = 200;           /* plenty for first 10 k terms */
    }
}

/* ---------- static state ---------- */
static seq_t *prime_seq   = NULL; /* primes in appearance order          */
static size_t prime_len   = 0;
static size_t prime_cap   = 0;

static seq_t *seen_sorted = NULL; /* primes already seen, sorted        */
static size_t seen_len    = 0;
static size_t seen_cap    = 0;

static char *cons_buf     = NULL; /* concatenated digits                 */
static size_t buf_len     = 0;
static size_t buf_cap     = 0;

static idx_t next_num     = 1;     /* next integer to append              */

/* ---------- memory helpers ---------- */
static void ensure_cap(seq_t **arr, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        *cap = (*cap == 0 ? 16 : (*cap << 1));
        *arr = (seq_t *)realloc(*arr, *cap * sizeof(seq_t));
    }
}

static int binary_search(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < val) lo = mid + 1;
        else if (arr[mid] > val) hi = mid;
        else return (int)mid;
    }
    return -1;
}

static void add_prime(seq_t val)
{
    /* find insertion point in ascending order */
    size_t lo = 0, hi = seen_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (seen_sorted[mid] < val) lo = mid + 1;
        else hi = mid;
    }
    ensure_cap(&seen_sorted, &seen_len, &seen_cap);
    memmove(&seen_sorted[lo + 1], &seen_sorted[lo],
            (seen_len - lo) * sizeof(seq_t));
    seen_sorted[lo] = val;
    ++seen_len;

    ensure_cap(&prime_seq, &prime_len, &prime_cap);
    prime_seq[prime_len++] = val;
}

/* ---------- core algorithm ---------- */
static void process_new_digit(size_t pos)
{
    compute_max_seq_digits();
    size_t max_len = pos + 1 < max_seq_digits ? pos + 1 : max_seq_digits;

    for (int l = (int)max_len; l >= 1; --l) {
        size_t start = pos + 1 - l;
        seq_t num = 0;
        for (size_t j = start; j <= pos; ++j) {
            num = num * 10 + (seq_t)(cons_buf[j] - '0');
        }
        if (num < 2) continue;
        if (binary_search(seen_sorted, seen_len, num) != -1) continue;
        if (!is_prime_seq(num)) continue;
        add_prime(num);
    }
}

static void append_integer(idx_t x)
{
    char tmp[32];
    size_t len = 0;
    if (x == 0) {
        tmp[len++] = '0';
    } else {
        while (x > 0) {
            tmp[len++] = (char)('0' + (x % 10));
            x /= 10;
        }
    }
    for (int i = (int)len - 1; i >= 0; --i) {
        if (buf_len == buf_cap) {
            buf_cap = (buf_cap ? buf_cap << 1 : 32);
            cons_buf = (char *)realloc(cons_buf, buf_cap);
        }
        cons_buf[buf_len++] = tmp[i];
        process_new_digit(buf_len - 1);
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defined only for n≥1 */

    while (prime_len < (size_t)n) {
        append_integer(next_num++);
    }
    return prime_seq[n - 1];
}
