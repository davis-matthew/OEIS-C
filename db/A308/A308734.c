/*
 * A308734  Number of ordered ways to write n as (2^a*3^b)^2 + (2^c*5^d)^2 +
 *          x^2 + y^2, where a,b,c,d,x,y are nonnegative integers with x <= y.
 * https://oeis.org/A308734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the ordered representations of n as (2^a*3^b)^2 +
 *            (2^c*5^d)^2 + x^2 + y^2, with a,b,c,d,x,y ≥ 0 and x ≤ y. The
 *            function computes all such representations by exhaustive
 *            enumeration of the (2^a*3^b) and (2^c*5^d) values and by
 *            pre‑computing the number of (x,y) pairs whose squares sum to...
 * Range:     u8 n <= 532, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ---- small helper: floor(sqrt(n)) for uint64_t --------------------- */
static size_t isqrt_u64(uint64_t n)
{
    if (n == 0) return 0;
    size_t low = 1, high = (size_t)n, ans = 0;
    while (low <= high) {
        size_t mid = low + ((high - low) >> 1);
        if (mid > n / mid) {
            if (mid == 0) break;          /* overflow guard */
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

/* ---- cache of already computed values ----------------------------- */
static seq_t *cache = NULL;          /* cache[n] holds a(n) for n ≤ cache_len */
static size_t cache_len = 0;         /* maximum index stored in cache */

/* ---- extend cache up to at least n -------------------------------- */
static void extend_cache(idx_t n)
{
    size_t new_len = (size_t)n;
    if (new_len <= cache_len) return;      /* nothing to do */

    /* limit for the multipliers 2^a*3^b and 2^c*5^d            */
    size_t limit = isqrt_u64(new_len);

    /* ---- generate (2^a*3^b)^2 values up to new_len ------------ */
    /* we know the count will be small, use a fixed buffer          */
    seq_t *s1 = (seq_t*)calloc(256, sizeof(seq_t));
    size_t s1cnt = 0;
    for (size_t a = 0, p2 = 1; p2 <= limit; ++a, p2 <<= 1) {
        for (size_t b = 0, val = p2; val <= limit; ++b, val *= 3) {
            if (s1cnt == 256) break;                 /* should not happen */
            s1[s1cnt++] = (seq_t)(val * val);
            if (val > limit / 3) break;
        }
    }

    /* ---- generate (2^c*5^d)^2 values up to new_len ------------ */
    seq_t *s2 = (seq_t*)calloc(256, sizeof(seq_t));
    size_t s2cnt = 0;
    for (size_t c = 0, p2 = 1; p2 <= limit; ++c, p2 <<= 1) {
        for (size_t d = 0, val = p2; val <= limit; ++d, val *= 5) {
            if (s2cnt == 256) break;                 /* should not happen */
            s2[s2cnt++] = (seq_t)(val * val);
            if (val > limit / 5) break;
        }
    }

    /* ---- pre‑compute number of (x,y) pairs with x≤y,
            x^2 + y^2 = r  --------------------------------------- */
    seq_t *sum_count = (seq_t*)calloc(new_len + 1, sizeof(seq_t));
    for (size_t x = 0; x * x <= new_len; ++x) {
        size_t x2 = x * x;
        for (size_t y = x; y * y <= new_len - x2; ++y) {
            size_t sum = x2 + y * y;
            sum_count[sum] += (seq_t)1;
        }
    }

    /* ---- compute all a(k) for k ≤ new_len --------------------- */
    seq_t *new_cache = (seq_t*)calloc(new_len + 1, sizeof(seq_t));
    for (size_t i = 0; i < s1cnt; ++i) {
        seq_t s1v = s1[i];
        for (size_t j = 0; j < s2cnt; ++j) {
            seq_t s2v = s2[j];
            size_t base = (size_t)(s1v + s2v);
            if (base > new_len) continue;
            size_t max_r = new_len - base;
            for (size_t r = 0; r <= max_r; ++r) {
                new_cache[base + r] += sum_count[r];
            }
        }
    }

    /* ---- free temporaries and update cache ------------------- */
    free(s1);
    free(s2);
    free(sum_count);
    free(cache);          /* old cache is discarded */
    cache = new_cache;
    cache_len = new_len;
}

/* ---- public sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is defined for n≥1 */
    if ((size_t)n > cache_len) extend_cache(n);
    return cache[n];
}
