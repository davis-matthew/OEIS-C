/*
 * A291346  a(n) = numbers that contain exactly four digits: three identical
 *          digits and another different digit.
 * https://oeis.org/A291346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 324, u32 n <= 324, u64 n <= 324, u128 n <= 324,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291346: numbers with exactly three identical digits and one distinct digit in base‑10.
   The function enumerates all 4‑digit numbers, retains those satisfying the condition,
   stores them in a static sorted array, and returns the nth (1‑based) element. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool is_valid(seq_t val) {
    uint8_t cnt[10] = {0};
    uint64_t t = (uint64_t)val;
    for (int i = 0; i < 4; ++i) {
        cnt[t % 10]++;
        t /= 10;
    }
    int three = -1, one = -1;
    for (int d = 0; d < 10; ++d) {
        if (cnt[d] == 3) three = d;
        else if (cnt[d] == 1) one = d;
    }
    return (three != -1 && one != -1);
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t count = 0;
    if (!cache) {
        /* precompute all terms – 324 terms known */
        size_t cap = 350;            /* enough for all terms */
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return (seq_t)0; /* allocation failure */
        size_t idx = 0;
        for (uint64_t num = 1000; num <= 9999 && idx < cap; ++num) {
            if (is_valid((seq_t)num)) {
                cache[idx++] = (seq_t)num;
            }
        }
        count = idx;
    }
    if (n == 0 || n > count) return (seq_t)0;
    return cache[n - 1];
}
