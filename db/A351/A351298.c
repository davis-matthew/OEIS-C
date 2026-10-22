/*
 * A351298  Concatenation of the lexicographically earliest 6-term closed
 *          circuits formed on a square grid by distinct segments of length
 *          a(n) at right angle.
 * https://oeis.org/A351298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 155, u16 n <= 1002, u32 n <= 1002, u64 n <= 1002,
 *            u128 n <= 1002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A351298 – Concatenation of the lexicographically earliest
    6‑term closed circuits on a unit square grid.
    Each circuit consists of positive integers (a1,a2,a3,a4,a5,a6)
    with the closure conditions
          a5 = a1 + a3      (vertical closure)
          a6 = a2 + a4      (horizontal closure)
    and all six integers distinct.
    The sequence is obtained by listing all such circuits in
    lexicographic order and concatenating their six integers.
    The implementation builds the sequence incrementally, keeping
    track of numbers that have already appeared.  */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/*  ---  Global state used for memoisation  ---  */
static seq_t *seq          = NULL;   /* concatenated sequence values   */
static size_t seq_cap      = 0;      /* capacity of seq array          */
static size_t seq_len      = 0;      /* number of values stored        */

static bool *used          = NULL;   /* bit‑style flag array of used values */
static size_t used_cap     = 0;      /* capacity of used array          */

/*  ---  Helper for dynamic allocation of the used flag array  ---  */
static inline void ensure_used(idx_t i)
{
    /*  Ensure that used[i] is valid.  */
    if ((size_t)(i + 1) > used_cap) {
        size_t new_cap = used_cap ? used_cap * 2 : 8;
        while ((size_t)(i + 1) > new_cap) new_cap <<= 1;
        bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!tmp) abort();
        /*  Initialise the new part of the array to false.  */
        for (size_t j = used_cap; j < new_cap; ++j) tmp[j] = false;
        used = tmp;
        used_cap = new_cap;
    }
}

static inline bool is_used(idx_t i)
{
    ensure_used(i);
    return used[i];
}

static inline void mark_used(idx_t i)
{
    ensure_used(i);
    used[i] = true;
}

/*  ---  Find the next lexicographically earliest circuit that
        uses only unused integers.  The six integers are written
        into buf[0]..buf[5].  ---  */
static void find_next_circuit(seq_t *buf)
{
    for (idx_t a1 = 1; ; ++a1) {
        if (is_used(a1)) continue;
        for (idx_t a2 = 1; ; ++a2) {
            if (is_used(a2) || a2 == a1) continue;
            for (idx_t a3 = 1; ; ++a3) {
                if (is_used(a3) || a3 == a1 || a3 == a2) continue;
                idx_t a5 = a1 + a3;
                if (a5 < a1 || a5 < a3) continue;           /* safety, never true */
                if (is_used(a5) || a5 == a1 || a5 == a2 || a5 == a3) continue;
                for (idx_t a4 = 1; ; ++a4) {
                    if (is_used(a4) ||
                        a4 == a1 || a4 == a2 || a4 == a3 || a4 == a5) continue;
                    idx_t a6 = a2 + a4;
                    if (a6 < a2 || a6 < a4) continue;           /* safety */
                    if (is_used(a6) ||
                        a6 == a1 || a6 == a2 || a6 == a3 ||
                        a6 == a4 || a6 == a5) continue;
                    /*  All six numbers are distinct and unused.  */
                    buf[0] = (seq_t)a1;
                    buf[1] = (seq_t)a2;
                    buf[2] = (seq_t)a3;
                    buf[3] = (seq_t)a4;
                    buf[4] = (seq_t)a5;
                    buf[5] = (seq_t)a6;
                    return;
                }
            }
        }
    }
}

/*  ---  Append a single value to the stored sequence, growing
        the array if necessary.  ---  */
static void append_value(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) abort();
    }
    seq[seq_len++] = val;
    /*  Mark the integer as used.  All values are positive
        integers, so we can safely convert to idx_t.  */
    mark_used((idx_t)val);
}

/*  ---  Public interface required by the benchmark framework.  ---  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset is 1 – undefined below */
    while (seq_len < n) {
        seq_t circ[6];
        find_next_circuit(circ);
        for (int i = 0; i < 6; ++i) append_value(circ[i]);
    }
    return seq[n - 1];
}
