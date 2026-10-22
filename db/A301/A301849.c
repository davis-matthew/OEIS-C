/*
 * A301849  The Pagoda sequence: a sequence with isolated zeros in its number
 *          wall over finite fields.
 * https://oeis.org/A301849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = b(n+1) - b(n-1), where b(n) denotes A038189(n)
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Pagoda sequence is obtained by coding the fixed point of the
   morphism 1→13, 2→23, 3→14, 4→24 with the block mapping:
   1 → {-1,-1,0,+1}
   2 → {0,-1,-1,+1}
   3 → {0,-1,+1,+1}
   4 → {+1,-1,0,+1}
   The (n+1)-th symbol of the sequence maps via this block to a(n). */
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

static const int first_sub[5] = {0, 1, 2, 1, 2};
static const int second_sub[5] = {0, 3, 3, 4, 4};

static int get_c(idx_t idx) {
    int s = 1;                     /* start from seed symbol 1 */
    if (idx == 0) return 1;
    idx_t pos = floor_log2_uint64(idx);
    for (idx_t i = pos; ; --i) {
        int bit = (int)((idx >> i) & 1);
        s = bit ? second_sub[s] : first_sub[s];
        if (i == 0) break;
    }
    return s;                       /* returns a value in {1,2,3,4} */
}

static const seq_t mapping[5][4] = {
    {0, 0, 0, 0},                 /* dummy, unused */
    {-1, -1,  0,  1},              /* 1 → {-1,-1,0,+1} */
    { 0, -1, -1,  1},              /* 2 → {0,-1,-1,+1} */
    { 0, -1,  1,  1},              /* 3 → {0,-1,+1,+1} */
    { 1, -1,  0,  1}               /* 4 → {+1,-1,0,+1} */
};

seq_t a(idx_t n) {
    idx_t m = n + 1;                 /* shift to align with a(-1) mapping */
    idx_t p = m / 4;                 /* index into the underlying c sequence */
    idx_t r = m % 4;                 /* position inside the 4‑symbol block */

    int sym = get_c(p);              /* c(p) ∈ {1,2,3,4} */
    seq_t val = mapping[sym][(int)r];
    return val;
}
