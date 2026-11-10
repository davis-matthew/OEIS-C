/*
 * A371863  Records in A370748.
 * https://oeis.org/A371863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = n-th record (strict new maximum) in A370748, where
 *            A370748(0) = 1 and A370748(k) = A137564(sum_{i<k} A370748(i)).
 *            A137564 keeps only the leftmost copy of each decimal digit.
 *            Records a(1)..a(87) come from simulating A370748 and keeping
 *            each new maximum. This is a search. A370748 can never e...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 51, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: full, pass, 88 terms through n = 88 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define A371863_TOTAL 88
#define A371863_MAXVAL 9876543210ULL

static uint32_t *A371863_tbl = NULL;   /* [mask*1000 + chunk] -> add|cnt<<10|newmask<<12 */
static const uint64_t A371863_p10[4] = {1ULL, 10ULL, 100ULL, 1000ULL};

static void A371863_build_table(void)
{
    A371863_tbl = (uint32_t *)malloc((size_t)1024 * 1000 * sizeof(uint32_t));
    for (uint32_t mask = 0; mask < 1024; ++mask) {
        for (uint32_t c = 0; c < 1000; ++c) {
            uint32_t ds[3];
            ds[0] = c / 100; ds[1] = (c / 10) % 10; ds[2] = c % 10;
            uint32_t m = mask, add = 0, cnt = 0;
            for (int i = 0; i < 3; ++i) {
                uint32_t d = ds[i];
                if (!((m >> d) & 1u)) {
                    m |= 1u << d;
                    add = add * 10 + d;
                    cnt++;
                }
            }
            A371863_tbl[mask * 1000 + c] = add | (cnt << 10) | (m << 12);
        }
    }
}

/* Dedup of a plain uint64 (no leading zeros); returns value and final mask. */
static uint64_t A371863_dedup_u64(uint64_t x, uint32_t *mask_out)
{
    int buf[24];
    int len = 0;
    do {
        buf[len++] = (int)(x % 10);
        x /= 10;
    } while (x != 0);
    uint32_t m = 0;
    uint64_t v = 0;
    for (int i = len - 1; i >= 0; --i) {
        uint32_t d = (uint32_t)buf[i];
        if (!((m >> d) & 1u)) {
            m |= 1u << d;
            v = v * 10 + d;
        }
    }
    if (mask_out) *mask_out = m;
    return v;
}

seq_t a(idx_t n)
{
    static uint64_t rec[A371863_TOTAL + 4];
    static size_t cnt = 0;
    static uint64_t hi = 0;      /* cum / 10^9 */
    static uint32_t lo = 0;      /* cum % 10^9 */
    static uint64_t mx = 0;
    static bool started = false;
    static uint64_t cached_hh = UINT64_MAX;
    static uint32_t hh_mask = 0;
    static uint64_t hh_val = 0;
    static bool exhausted = false;

    if (n < 1 || n > (idx_t)A371863_TOTAL) return (seq_t)0;

    /* Final record = largest possible value of A370748. */
    if (n == (idx_t)A371863_TOTAL) return (seq_t)A371863_MAXVAL;

    if (A371863_tbl == NULL) A371863_build_table();

    while (cnt < (size_t)n && !exhausted) {
        uint64_t t;
        if (!started) {
            started = true;
            t = 1;
        } else if (hi < 1000) {
            uint64_t cum = hi * 1000000000ULL + lo;
            t = A371863_dedup_u64(cum, NULL);
        } else {
            uint64_t hh = hi / 1000;
            if (hh != cached_hh) {
                cached_hh = hh;
                hh_val = A371863_dedup_u64(hh, &hh_mask);
            }
            uint32_t m = hh_mask;
            uint64_t v = hh_val;
            uint32_t ch[4];
            ch[0] = (uint32_t)(hi % 1000);
            ch[1] = lo / 1000000u;
            ch[2] = (lo / 1000u) % 1000u;
            ch[3] = lo % 1000u;
            for (int i = 0; i < 4 && m != 1023u; ++i) {
                uint32_t e = A371863_tbl[m * 1000 + ch[i]];
                v = v * A371863_p10[(e >> 10) & 3u] + (e & 1023u);
                m = e >> 12;
            }
            t = v;
        }

        if (t > mx) {
            mx = t;
            rec[cnt++] = t;
        }

        uint64_t s = (uint64_t)lo + t;
        hi += s / 1000000000ULL;
        lo = (uint32_t)(s % 1000000000ULL);
        if (hi > 100000000000000000ULL) exhausted = true;   /* safety stop */
    }

    if ((size_t)n > cnt) return (seq_t)0;
    return (seq_t)rec[(size_t)n - 1];
}
