/*
 * A338111  Times displayed on an hour|minute 12-hour 7-segment digital clock,
 *          arranged in order of increasing brightness (see Comments).
 * https://oeis.org/A338111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 720, u32 n <= 720, u64 n <= 720,
 *            u128 n <= 720, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code: a(n) is the n-th 12‑hour minute time sorted by the total number
   of 7‑segment lights, ties broken chronologically. */

#include <stddef.h>
#include <stdlib.h>

static const unsigned char _DIGIT_SEG[10] = {6,2,5,5,4,5,6,3,7,6};

struct TimeEntry {
    seq_t val;
    unsigned char seg;
    idx_t idx;        /* chronological order */
};

static int cmp_timeentry(const void *p, const void *q)
{
    const struct TimeEntry *a = (const struct TimeEntry *)p;
    const struct TimeEntry *b = (const struct TimeEntry *)q;
    if (a->seg != b->seg)
        return a->seg < b->seg ? -1 : 1;
    if (a->idx < b->idx) return -1;
    if (a->idx > b->idx) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    static seq_t cache[720];
    static int initialized = 0;
    if (!initialized) {
        struct TimeEntry entries[720];
        idx_t pos = 0;
        for (int h = 1; h <= 12; ++h) {
            for (int m = 0; m < 60; ++m) {
                unsigned int seg = 0;
                if (h >= 10) {
                    seg += _DIGIT_SEG[1];
                    seg += _DIGIT_SEG[h % 10];
                } else {
                    seg += _DIGIT_SEG[h];
                }
                seg += _DIGIT_SEG[m / 10];
                seg += _DIGIT_SEG[m % 10];
                seq_t val = (seq_t)(h * 100 + m);
                entries[pos].val = val;
                entries[pos].seg = (unsigned char)seg;
                entries[pos].idx = pos;
                ++pos;
            }
        }
        qsort(entries, 720, sizeof(entries[0]), cmp_timeentry);
        for (idx_t i = 0; i < 720; ++i) {
            cache[i] = entries[i].val;
        }
        initialized = 1;
    }
    if (n < 1 || n > 720) return 0;
    return cache[n - 1];
}
