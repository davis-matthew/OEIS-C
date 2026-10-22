/*
 * A327226  For any n >= 0, let u and v be such that 2 <= u < v and the digits
 *          of n in bases u and v are the same up to a permutation and v is
 *          minimized; a(n) = v.
 * https://oeis.org/A327226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal v > u ≥ 2 such that the digits of n in bases u
 *            and v are the same up to a permutation. The function computes
 *            the value by generating all base representations of n (bases 2
 *            .. n+2), sorting the representations, and finding the least v
 *            that appears with a smaller u
 * Range:     u8 n <= 574, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies: standard C library only. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint16_t base;      // base (2 … n+2)
    uint8_t  len;       // number of digits
    uint16_t digits[14];/* digits sorted increasingly (max 14 for n ≤ 10000) */
} entry_t;

/* comparator for qsort: first by length, then lexicographically by digits */
static int cmp_entries(const void *p1, const void *p2) {
    const entry_t *e1 = (const entry_t *)p1;
    const entry_t *e2 = (const entry_t *)p2;
    if (e1->len != e2->len) return (int)(e1->len - e2->len);
    for (int i = 0; i < e1->len; ++i) {
        if (e1->digits[i] != e2->digits[i]) return (e1->digits[i] > e2->digits[i]) ? 1 : -1;
    }
    return 0;
}

seq_t a(idx_t n) {
    /* Special cases */
    if (n == 0) return (seq_t)3;     /* a(0) = 3 by definition */
    /* For n == 1 the algorithm below works fine; no need to special case. */

    /* Number of bases we need to consider: bases 2 … n+2 inclusive */
    size_t base_count = (size_t)(n + 1);   /* (n+2)-2+1 = n+1 */
    entry_t *entries = (entry_t *)malloc(base_count * sizeof(entry_t));
    if (!entries) return (seq_t)0;   /* out of memory: return 0 as a safe fallback */

    for (size_t i = 0; i < base_count; ++i) {
        uint16_t base = (uint16_t)(i + 2);   /* base 2 … n+2 */
        seq_t val = (seq_t)n;
        uint8_t len = 0;

        if (n == 0) {
            /* Representation of 0 is a single digit 0 */
            entries[i].digits[0] = 0;
            len = 1;
        } else {
            /* Compute digits of n in base `base` (least significant first) */
            while (val > 0) {
                uint16_t digit = (uint16_t)(val % base);
                entries[i].digits[len++] = digit;
                val /= base;
            }
        }

        /* Sort digits increasingly using simple insertion sort (len ≤ 14) */
        for (int j = 1; j < (int)len; ++j) {
            uint16_t key = entries[i].digits[j];
            int k = j - 1;
            while (k >= 0 && entries[i].digits[k] > key) {
                entries[i].digits[k + 1] = entries[i].digits[k];
                --k;
            }
            entries[i].digits[k + 1] = key;
        }

        entries[i].base = base;
        entries[i].len  = len;
    }

    /* Sort entries by representation */
    qsort(entries, base_count, sizeof(entry_t), cmp_entries);

    /* Scan sorted entries to find minimal v */
    seq_t min_v = (seq_t)(n + 2);   /* worst‑case bound */
    size_t pos = 0;
    while (pos < base_count) {
        size_t next = pos + 1;
        while (next < base_count && cmp_entries(&entries[pos], &entries[next]) == 0)
            ++next;

        if (next - pos >= 2) { /* at least two bases share the same digit multiset */
            seq_t min_base = UINT64_MAX;
            seq_t second_min = UINT64_MAX;
            for (size_t k = pos; k < next; ++k) {
                seq_t b = (seq_t)entries[k].base;
                if (b < min_base) {
                    second_min = min_base;
                    min_base   = b;
                } else if (b < second_min) {
                    second_min = b;
                }
            }
            if (second_min < min_v) min_v = second_min;
        }
        pos = next;
    }

    free(entries);
    return (seq_t)min_v;
}
