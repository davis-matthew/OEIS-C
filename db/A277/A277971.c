/*
 * A277971  a(1) = 1; a(n) > a(n-1) is the smallest number whose name in
 *          English contains the first letter of the name of a(n-1).
 * https://oeis.org/A277971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A277971:
   a(1) = 1; for n > 1, a(n) is the smallest integer > a(n‑1)
   whose English name contains the first letter of the English name
   of a(n‑1).  The implementation uses memoisation and brute‑force
   search for the next term. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef __cplusplus

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}
#endif

/* ----- English name helpers ----- */
static const char *const unit_names[] = {
    "zero", "one", "two", "three", "four", "five", "six",
    "seven", "eight", "nine", "ten", "eleven", "twelve",
    "thirteen", "fourteen", "fifteen", "sixteen", "seventeen",
    "eighteen", "nineteen"
};
static const char *const tens_names[] = {
    "", "", "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"
};

static void build_name(uint64_t n, char *buf)
{
    if (n >= 1000000000ULL) {
        build_name(n / 1000000000ULL, buf);
        strcat(buf, " billion");
        if (n % 1000000000ULL) {
            strcat(buf, " ");
            build_name(n % 1000000000ULL, buf);
        }
        return;
    }
    if (n >= 1000000ULL) {
        build_name(n / 1000000ULL, buf);
        strcat(buf, " million");
        if (n % 1000000ULL) {
            strcat(buf, " ");
            build_name(n % 1000000ULL, buf);
        }
        return;
    }
    if (n >= 1000ULL) {
        build_name(n / 1000ULL, buf);
        strcat(buf, " thousand");
        if (n % 1000ULL) {
            strcat(buf, " ");
            build_name(n % 1000ULL, buf);
        }
        return;
    }
    if (n >= 100ULL) {
        build_name(n / 100ULL, buf);
        strcat(buf, " hundred");
        if (n % 100ULL) {
            strcat(buf, " ");
            build_name(n % 100ULL, buf);
        }
        return;
    }
    if (n >= 20ULL) {
        strcat(buf, tens_names[n / 10ULL]);
        if (n % 10ULL) {
            strcat(buf, " ");
            build_name(n % 10ULL, buf);
        }
        return;
    }
    if (n > 0ULL) {
        strcat(buf, unit_names[n]);
        return;
    }
    /* n == 0 never occurs for numbers used here */
}

static char first_letter_of_value(uint64_t n)
{
    static char buf[512];
    buf[0] = '\0';
    build_name(n, buf);
    for (char *p = buf; *p; ++p) {
        if (*p != ' ')
            return *p;
    }
    return '\0';
}

static int contains_letter_in_value(uint64_t n, char letter)
{
    static char buf[512];
    buf[0] = '\0';
    build_name(n, buf);
    for (char *p = buf; *p; ++p) {
        if (*p == letter)
            return 1;
    }
    return 0;
}

/* ----- Cache management ----- */
static seq_t *cache = NULL;    /* cached values */
static idx_t cache_len = 0;    /* how many values stored */
static idx_t cache_cap = 0;    /* current allocation size */

static void ensure_cache(idx_t needed)
{
    if (!cache) {
        /* first allocation */
#ifdef __cplusplus
        cache = seq_alloc(needed);
#else
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
#endif
        cache_cap = needed;
    } else if (needed > cache_cap) {
#ifdef __cplusplus
        cache = seq_realloc(cache, cache_cap, needed);
#else
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
#endif
        cache_cap = needed;
    }
}

/* ----- Main function ----- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for 0, but safe */

    if (n <= cache_len)
        return cache[n - 1]; /* already computed */

    ensure_cache(n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else {
            seq_t prev  = cache[i - 2];
            uint64_t prev_u = (uint64_t)prev;     /* cast for name generation */
            char target = first_letter_of_value(prev_u);

            seq_t cand = prev + 1;
            while (!contains_letter_in_value((uint64_t)cand, target))
                cand = cand + 1;
            val = cand;
        }
        cache[i - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
