/*
 * A374995  Total cost when the elements of the n-th composition (in standard
 *          order) are requested from a self-organizing list initialized to
 *          (1, 2, 3, ...), where a requested element at position i is moved
 *          to position floor((i+1)/2).
 * https://oeis.org/A374995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is computed by simulating the self‑organising list for the n‑th
   composition (in standard order).  For an index n ≥ 1, the n‑th
   composition is the (n - (2^(s-1) − 1))-th composition of the integer
   s = ⌈log₂(n+1)⌉, enumerated in ascending binary order.  The cost of
   each requested element equals its current position in the list; after
   a request, the element is moved to index ⌊i/2⌋ (0‑based). */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* helper: compute the number of bits of a positive integer */
static inline idx_t bitlen(idx_t x)
{
    idx_t len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

seq_t a(idx_t n)
{
    if (n == 0u) {
        return (seq_t)0; /* a(0) = 0 */
    }

    /* Determine the sum s of the composition */
    idx_t s = bitlen(n);          /* s = ceil(log₂(n+1)) */
    idx_t prev = ((idx_t)1 << (s - 1)) - 1; /* cumulative up to s-1 */
    idx_t pos  = n - prev;         /* 1‑based position within compositions of s */

    /* Compute the binary representation of pos-1 to generate parts */
    idx_t bits_needed = s - 1;
    idx_t val_bits    = pos - 1;
    /* parts of the composition: at most s parts, each ≤ s (≤ 64) */
    int  parts[64];
    int  part_cnt = 0;
    int  current = 1;

    for (idx_t i = 0; i < bits_needed; ++i) {
        /* Bits are processed from most significant to least */
        int bit = (int)((val_bits >> (bits_needed - 1 - i)) & 1ULL);
        if (bit == 0) {
            ++current;                     /* continue current part */
        } else {
            parts[part_cnt++] = current;   /* cut here */
            current = 1;
        }
    }
    parts[part_cnt++] = current;           /* final part */

    /* Simulate the self‑organising list */
    int        list[64];     /* current list contents */
    int        idx_of[65];   /* mapping value -> current index */
    for (idx_t i = 0; i < s; ++i) {
        list[i]   = (int)(i + 1);   /* values 1..s */
        idx_of[i + 1] = (int)i;     /* indices 0..s-1 */
    }

    seq_t cost = (seq_t)0;
    for (int p = 0; p < part_cnt; ++p) {
        int val = parts[p];
        int idx = idx_of[val];
        cost += (seq_t)(idx + 1);   /* position is 1‑based */

        int target = idx / 2;       /* floor(idx/2) */
        if (target != idx) {
            int v_move = list[idx];
            /* shift elements right to make room at target */
            memmove(&list[target + 1], &list[target],
                    (size_t)(idx - target) * sizeof(int));
            list[target] = v_move;

            /* update indices of shifted elements */
            for (int j = (int)idx; j > target; --j) {
                int mv = list[j];
                idx_of[mv] = j;
            }
            idx_of[v_move] = target;
        }
    }

    return cost;
}
