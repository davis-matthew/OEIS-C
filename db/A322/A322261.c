/*
 * A322261  Square array T(n, k) (n >= 0, k >= 0) read by antidiagonals
 *          upwards: the lengths of runs in binary expansion of T(n, k)
 *          correspond to the lengths of runs in binary expansion of n
 *          followed by the lengths of runs in binary expansion of k.
 * https://oeis.org/A322261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A322261
   Square array T(n,k) read by antidiagonals upwards:
   a(n) = T(i,j) where (i,j) are the coordinates of the n-th element
   when each diagonal is traversed from the top‑right to the bottom‑left.
   The binary run‑lengths of T(i,j) are the concatenation of the run‑lengths
   of the binary expansions of i and j, starting with a run of 1s.
*/

#include "../utils/bitlen_seq_t.h" /* for bitlen_seq_t() */

/* Extract the run lengths of x into runs[0..max-1].
   Returns the number of runs.  Runs are counted from the most
   significant bit.  For x==0, returns 0.  The runs correspond to
   consecutive identical bits, starting with a run of 1s. */
static inline int get_runs(seq_t x, int *runs, int max)
{
    if (x == (seq_t)0) return 0;

    unsigned bl = bitlen_seq_t(x);          /* number of bits in x   */
    int cur_bit = (int)((x >> (bl - 1)) & 1);
    int count   = 1;
    int idx     = 0;

    for (int i = (int)bl - 2; i >= 0 && idx < max; --i) {
        int bit = (int)((x >> i) & 1);
        if (bit == cur_bit) {
            ++count;
        } else {
            runs[idx++] = count;
            count = 1;
            cur_bit = bit;
        }
    }
    if (idx < max) runs[idx++] = count;
    return idx;
}

/* Build T(n,k) from the run lengths of n and k. */
static inline seq_t make_T(seq_t n, seq_t k)
{
    /* Allocate space for the run lengths: at most 2*64 runs for 64‑bit inputs. */
    int runs_n[128], runs_k[128];
    int rn = get_runs(n, runs_n, 128);
    int rk = get_runs(k, runs_k, 128);

    seq_t val = 0;
    int bit = 1;          /* first run is of 1s */

    for (int i = 0; i < rn; ++i) {
        int L = runs_n[i];
        for (int t = 0; t < L; ++t)
            val = (val << 1) | (seq_t)bit;
        bit ^= 1;
    }

    for (int i = 0; i < rk; ++i) {
        int L = runs_k[i];
        for (int t = 0; t < L; ++t)
            val = (val << 1) | (seq_t)bit;
        bit ^= 1;
    }

    return val;
}

/* Convert sequence index n to the pair (i,j) of the array T,
   traversing each diagonal from top‑right to bottom‑left. */
static inline void antidiagonal(idx_t idx, idx_t *i, idx_t *j)
{
    idx_t d = 0;
    idx_t acc = 0;      /* total terms processed so far */
    while (idx >= acc + d + 1) {
        acc += d + 1;
        ++d;
    }
    idx_t offset = idx - acc;
    *i = d - offset;
    *j = offset;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal(n, &i, &j);
    return make_T((seq_t)i, (seq_t)j);
}
