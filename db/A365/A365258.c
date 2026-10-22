/*
 * A365258  The four digits of a(n), their three successive absolute first
 *          differences and their two successive absolute second differences
 *          are all distinct.
 * https://oeis.org/A365258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   A365258 is obtained by brute force: 4‑digit numbers with
 *            non‑zero distinct digits 1‑9, whose first and second absolute
 *            differences are also distinct digits 1‑9. The 18 valid numbers
 *            are sorted ascending
 * Range:     u8 none, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static int cmp_uint64(const void *a, const void *b) {
    uint64_t va = *(const uint64_t*)a;
    uint64_t vb = *(const uint64_t*)b;
    return (va < vb) ? -1 : (va > vb);
}

seq_t a(idx_t n) {
    static seq_t cache[19];          /* 1‑based, cache[0] unused */
    static bool init = false;
    if (!init) {
        uint64_t tmp[200];          /* space for all candidates */
        size_t count = 0;
        for (uint64_t num = 1234; num <= 9876; ++num) {
            uint8_t d[4];
            uint64_t x = num;
            d[3] = x % 10; x /= 10;
            d[2] = x % 10; x /= 10;
            d[1] = x % 10; x /= 10;
            d[0] = x % 10;
            if (d[0]==0 || d[1]==0 || d[2]==0 || d[3]==0) continue;

            uint16_t mask = 0;
            bool ok = true;
            for (int i = 0; i < 4; ++i) {
                uint16_t bit = 1u << (d[i]-1);
                if (mask & bit) { ok = false; break; }
                mask |= bit;
            }
            if (!ok) continue;

            uint8_t f[3];
            f[0] = (d[1] > d[0]) ? d[1]-d[0] : d[0]-d[1];
            f[1] = (d[2] > d[1]) ? d[2]-d[1] : d[1]-d[2];
            f[2] = (d[3] > d[2]) ? d[3]-d[2] : d[2]-d[3];
            for (int i = 0; i < 3; ++i) {
                if (f[i]==0 || f[i]>9) { ok = false; break; }
                uint16_t bit = 1u << (f[i]-1);
                if (mask & bit) { ok = false; break; }
                mask |= bit;
            }
            if (!ok) continue;

            uint8_t s[2];
            s[0] = (f[1] > f[0]) ? f[1]-f[0] : f[0]-f[1];
            s[1] = (f[2] > f[1]) ? f[2]-f[1] : f[1]-f[2];
            for (int i = 0; i < 2; ++i) {
                if (s[i]==0 || s[i]>9) { ok = false; break; }
                uint16_t bit = 1u << (s[i]-1);
                if (mask & bit) { ok = false; break; }
                mask |= bit;
            }
            if (!ok) continue;

            if (mask == 0x1FFu) {          /* 9 distinct digits 1‑9 */
                tmp[count++] = num;
            }
        }
        qsort(tmp, count, sizeof(uint64_t), cmp_uint64);
        for (size_t i = 0; i < count; ++i) {
            cache[i+1] = (seq_t)tmp[i];
        }
        cache[count+1] = 0;
        init = true;
    }
    if (n == 0 || n > 18) return 0;
    return cache[n];
}
