/*
 * A297315  Number of 2 X n 0..1 arrays with every 1 horizontally or
 *          antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A297315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 8*a(n-3) - 3*a(n-4) + 7*a(n-5) -
 *            5*a(n-6) - a(n-7) - a(n-8) + a(n-9); Initial values: a(1)=1,
 *            a(2)=7, a(3)=23, a(4)=66, a(5)=207, a(6)=654, a(7)=2049,
 *            a(8)=6422, a(9)=20119
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static const seq_t INIT_VALUES[9] = {
    (seq_t)1,
    (seq_t)7,
    (seq_t)23,
    (seq_t)66,
    (seq_t)207,
    (seq_t)654,
    (seq_t)2049,
    (seq_t)6422,
    (seq_t)20119
};

static seq_t *MEM = NULL;
static idx_t MEM_SIZE = 0;
static idx_t MEM_CAP  = 0;

static void init_cache(void) {
    if (MEM == NULL) {
        MEM_CAP  = 9;
        MEM_SIZE = 9;
        MEM = (seq_t*)malloc(MEM_CAP * sizeof(seq_t));
        for (idx_t i = 0; i < 9; ++i) MEM[i] = INIT_VALUES[i];
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1, not used here */

    if (n <= 9) return INIT_VALUES[n-1];

    init_cache();

    if (n <= MEM_SIZE) return MEM[n-1];

    for (idx_t cur = MEM_SIZE + 1; cur <= n; ++cur) {
        /* growth */
        if (cur > MEM_CAP) {
            idx_t new_cap = MEM_CAP * 2;
            while (new_cap < cur) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(MEM, new_cap * sizeof(seq_t));
            if (tmp == NULL) abort();     /* allocation failed */
            MEM = tmp;
            MEM_CAP = new_cap;
        }

        /* compute using positive/negative parts to avoid underflow on unsigned types */
        seq_t pos = 3 * MEM[cur-2];
        pos += 8 * MEM[cur-4];
        pos += 7 * MEM[cur-6];
        pos += MEM[cur-10];

        seq_t neg = 2 * MEM[cur-3];
        neg += 3 * MEM[cur-5];
        neg += 5 * MEM[cur-7];
        neg += MEM[cur-8];
        neg += MEM[cur-9];

        seq_t val = pos - neg;        /* guaranteed non‑negative for this recurrence */

        MEM[cur-1] = val;
    }
    MEM_SIZE = n;
    return MEM[n-1];
}
