/*
 * A395103  Numbers with exactly k prime factors, counting repeats, with k>1,
 *          that are formed by concatenating k primes.
 * https://oeis.org/A395103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A395103 = numbers with exactly k>1 prime factors (counting *
 *            multiplicity) that can be written as the concatenation of k
 *            primes. * This implementation searches integers incrementally
 * Range:     u8 n <= 19, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   bigomega_seq, is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <stdint.h>
#include <stddef.h>
#include "../utils/bigomega_seq.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* ------------------------------------------------------------------ */
/*  Helper: recursive split into `parts_left` primes.                  */
/* ------------------------------------------------------------------ */
static bool dfs_split(const char *s, size_t pos, int parts_left, size_t total_len)
{
    if (parts_left == 0)
        return pos == total_len;
    if (pos == total_len)
        return false;
    if (parts_left > (int)(total_len - pos))
        return false;                           /* not enough digits left */

    if (s[pos] == '0')
        return false;                           /* leading zero – invalid prime */

    for (size_t end = pos + 1; end <= total_len; ++end) {
        /* Temporary buffer for the substring; 512 is ample for our search. */
        static char subbuf[512];
        size_t sublen = end - pos;
        if (sublen >= sizeof(subbuf))
            continue;                           /* substring too long, skip */

        memcpy(subbuf, s + pos, sublen);
        subbuf[sublen] = '\0';

        seq_t part = seq_from_str(subbuf);
        if (is_prime_big(part) && dfs_split(s, end, parts_left - 1, total_len))
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Test whether `x` can be split into `k` prime parts.                  */
/* ------------------------------------------------------------------ */
static bool can_split_into_k_primes(seq_t x, idx_t k)
{
    if (k < 2)
        return false;

    size_t len = num_digits(x);
    if (len < (size_t)k)
        return false;                           /* not enough digits */

    /* Allocate enough space for decimal representation. */
    char *s = (char *)malloc(len + 1);
    if (!s)
        abort();
    seq_to_str(x, s, &len);                      /* len is updated but unchanged */

    bool ok = dfs_split(s, 0, (int)k, len);

    free(s);
    return ok;
}

/* ------------------------------------------------------------------ */
/*  Cache all found terms.                                             */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;      /* array of cached terms */
static size_t cache_capacity = 0;
static size_t cache_count    = 0;
static seq_t current_value = 21;  /* first candidate is 22 */

/* ------------------------------------------------------------------ */
/*  Main sequence function.                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Fill cache until we have at least `n` terms. */
    while (cache_count < n) {
        ++current_value;                       /* next integer to test     */

        seq_t k_seq = bigomega_seq(current_value);
        if (k_seq < (seq_t)2)
            continue;                           /* skip numbers with ≤1 prime factor */

        idx_t k = (idx_t)k_seq;
        if (!can_split_into_k_primes(current_value, k))
            continue;                           /* split condition failed    */

        /* Ensure capacity. */
        if (cache_count >= cache_capacity) {
            size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
                abort();
            cache       = tmp;
            cache_capacity = new_cap;
        }

        cache[cache_count++] = current_value;
    }

    return cache[n - 1];
}
