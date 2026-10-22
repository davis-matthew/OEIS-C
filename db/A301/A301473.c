/*
 * A301473  Summarize the square of the previous term (digits in increasing
 *          order), starting with a(1) = 1.
 * https://oeis.org/A301473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Summarize the square of the previous term (digits in increasing order),
 *   starting with a(1) = 1.
 *   a(n) is built by squaring a(n‑1), counting the frequency of each
 *   decimal digit in that square, and concatenating count and digit in
 *   ascending digit order.  The implementation uses arbitrary‑precision
 *   decimal arithmetic via strings so that an overflow in the intermediate
 *   square never corrupts the result. */

/*  Standard headers  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#ifdef __cplusplus
#include <string>
#include <vector>
#endif

/* Forward declaration of cache routine */
static void ensure_cache(idx_t n);

/* ---------- C++ implementation ---------- */
#ifdef __cplusplus

/* Global cache: vector of seq_t values and of their decimal string
 * representations.  The vector indices are 0‑based but a(1) is cache[0].  */
static std::vector<seq_t> cache;
static std::vector<std::string> str_cache;

/* Helper: multiply two decimal strings (a == b) and return a newly
 * allocated string containing the result in normal order.  */
static std::string mul_string(const std::string &s)
{
    size_t len = s.size();
    std::vector<int> prod(len * 2 + 1, 0);

    for (size_t i = 0; i < len; ++i) {
        int ai = s[len - 1 - i] - '0';
        for (size_t j = 0; j < len; ++j) {
            int bj = s[len - 1 - j] - '0';
            prod[i + j] += ai * bj;
        }
    }

    for (size_t k = 0; k < prod.size(); ++k) {
        if (prod[k] >= 10) {
            int c = prod[k] / 10;
            prod[k] %= 10;
            prod[k + 1] += c;
        }
    }

    size_t k = prod.size() - 1;
    while (k > 0 && prod[k] == 0) --k;

    std::string res;
    res.reserve(k + 1);
    for (size_t i = k; i != (size_t)-1; --i) res += char('0' + prod[i]);

    return res;
}

/* Helper: convert a decimal string to seq_t.  */
static seq_t str_to_seq(const std::string &s)
{
    seq_t val = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        val = val * 10 + (seq_t)(s[i] - '0');
    }
    return val;
}

/* Expand the cache up to and including term n. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;          /* a(0) is undefined, keep cache empty */

    if (cache.size() >= (size_t)n) return;

    /* Initialise from a(1) if the cache is empty */
    if (cache.empty()) {
        cache.push_back((seq_t)1);
        str_cache.emplace_back("1");
    }

    while (cache.size() < (size_t)n) {
        const std::string &prev_str = str_cache.back();
        /* Step 1: compute square of the previous term */
        std::string sq_str = mul_string(prev_str);

        /* Step 2: count digit frequencies */
        unsigned long long cnt[10] = {0};
        for (char c : sq_str) cnt[c - '0']++;

        /* Step 3: build the new term as a decimal string */
        std::string new_str;
        char tmp[64];
        for (int d = 0; d <= 9; ++d) {
            if (cnt[d] == 0) continue;
            /* append count */
            snprintf(tmp, sizeof(tmp), "%llu", cnt[d]);
            new_str += tmp;
            /* append the digit itself */
            new_str += char('0' + d);
        }

        /* Step 4: convert to seq_t and store in cache */
        cache.push_back(str_to_seq(new_str));
        str_cache.emplace_back(std::move(new_str));
    }
}

/* Sequence function  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined */
    ensure_cache(n);
    return cache[n - 1];
}

#else  /* ---------- C implementation ---------- */

/* Cache data structures */
static seq_t *cache = NULL;
static char **str_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static size_t str_cap = 0;

/* Helper: multiply two decimal strings (numbers are identifiers of length len)
 * and return a newly allocated heap string.  Caller must free the result. */
static char* mul_string(const char *s)
{
    size_t len = strlen(s);
    size_t prod_len = len * 2 + 1;
    unsigned int *prod = calloc(prod_len, sizeof(unsigned int));
    for (size_t i = 0; i < len; ++i) {
        unsigned int ai = s[len - 1 - i] - '0';
        for (size_t j = 0; j < len; ++j) {
            unsigned int bj = s[len - 1 - j] - '0';
            prod[i + j] += ai * bj;
        }
    }
    for (size_t k = 0; k < prod_len; ++k) {
        if (prod[k] >= 10) {
            unsigned int c = prod[k] / 10;
            prod[k] %= 10;
            prod[k + 1] += c;
        }
    }
    size_t k = prod_len - 1;
    while (k > 0 && prod[k] == 0) --k;
    char *res = malloc(k + 2);
    for (size_t i = k; i != (size_t)-1; --i) res[k - i] = '0' + prod[i];
    res[k + 1] = '\0';
    free(prod);
    return res;
}

/* Helper: convert decimal string to seq_t */
static seq_t str_to_seq(const char *s)
{
    seq_t val = 0;
    for (size_t i = 0; s[i]; ++i) {
        val = val * 10 + (seq_t)(s[i] - '0');
    }
    return val;
}

/* Expand cache */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    if (cache_len >= (size_t)n) return;

    /* Grow numerical cache if needed */
    if (cache_len == 0) {
        cache = realloc(cache, sizeof(seq_t) * 16);
        cache_cap = 16;
    }
    if (cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        cache = realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    /* Grow string cache if needed */
    if (str_cache == NULL || str_cap < n) {
        size_t new_cap = str_cap ? str_cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        str_cache = realloc(str_cache, sizeof(char*) * new_cap);
        str_cap = new_cap;
    }

    /* Initialise */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        str_cache[0] = strdup("1");
        cache_len = 1;
    }

    /* Compute terms up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        const char *prev_str = str_cache[i - 1];

        /* Step 1: square */
        char *sq_str = mul_string(prev_str);

        /* Step 2: count digits */
        unsigned long long cnt[10] = {0};
        for (char *p = sq_str; *p; ++p) cnt[*p - '0']++;

        /* Step 3: build new string */
        size_t bufsize = 128;
        char *new_str = malloc(bufsize);
        size_t pos = 0;
        char tmp[64];
        for (int d = 0; d <= 9; ++d) {
            if (cnt[d] == 0) continue;
            int len = snprintf(tmp, sizeof(tmp), "%llu", cnt[d]);
            if (pos + len + 1 >= bufsize) {
                bufsize = (pos + len + 1) * 2;
                new_str = realloc(new_str, bufsize);
            }
            memcpy(new_str + pos, tmp, len);
            pos += len;
            new_str[pos++] = '0' + d;
        }
        new_str[pos] = '\0';

        /* Step 4: convert to seq_t and store */
        cache[i] = str_to_seq(new_str);
        str_cache[i] = new_str;

        free(sq_str);
    }
    cache_len = n;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_cache(n);
    return cache[n - 1];
}

#endif
