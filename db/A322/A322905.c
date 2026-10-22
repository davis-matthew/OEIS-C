/*
 * A322905  Sequence consists of all pairs of numbers x and y such that x is
 *          the reverse of y, and there exist numbers i and j such that x =
 *          i-j and y=i*j; the list of the numbers x and y is then sorted into
 *          ascending order and duplicates are removed.
 * https://oeis.org/A322905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n‑th number in the OEIS sequence A322905. The
 *            sequence is generated from the patterns - For n%4==0 : 147 +
 *            (5247)^(k‑1) + 5244 (k = n/4) - For n%4==1 : reverse of the
 *            above (k = (n‑1)/4) - For n%4==2 : 161247 + (5247)^(k‑1) + 384
 *            (k = (n‑2)/4) - For n%4==3 : reverse of the above (k =...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse.h"

static seq_t block_concat0 = 0;          /* 5247 repeated (k‑1) for residue 0 */
static seq_t pow10_block_len0 = 1;       /* 10^(4*(k‑1)) for residue 0 */
static idx_t curr_k0 = 1;                /* next k to be produced for residue 0 */

static seq_t block_concat2 = 0;          /* 5247 repeated (k‑1) for residue 2 */
static seq_t pow10_block_len2 = 1;       /* 10^(4*(k‑1)) for residue 2 */
static idx_t curr_k2 = 1;                /* next k to be produced for residue 2 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 144;
    if (n == 3) return 441;

    unsigned rem = (unsigned)(n % 4);
    seq_t val;

    if (rem == 0) {                     /* residue 0 */
        idx_t k = n / 4;
        /* advance stored state to k */
        while (curr_k0 < k) {
            block_concat0 = block_concat0 * 10000 + 5247;
            pow10_block_len0 = pow10_block_len0 * 10000;
            curr_k0++;
        }
        seq_t shift = pow10_block_len0 * 10000;      /* 10^(4*(k‑1)+4) */
        seq_t part1 = 147 * shift;
        seq_t part2 = block_concat0 * 10000;
        val = part1 + part2 + 5244;
    }
    else if (rem == 2) {                /* residue 2 */
        idx_t k = (n - 2) / 4;
        while (curr_k2 < k) {
            block_concat2 = block_concat2 * 10000 + 5247;
            pow10_block_len2 = pow10_block_len2 * 10000;
            curr_k2++;
        }
        seq_t shift = pow10_block_len2 * 1000;       /* 10^(4*(k‑1)+3) */
        seq_t part1 = 161247 * shift;
        seq_t part2 = block_concat2 * 1000;
        val = part1 + part2 + 384;
    }
    else { /* rem == 1 or 3: reverse of the corresponding base number */
        idx_t k = (rem == 1) ? (n - 1) / 4 : (n - 3) / 4;
        seq_t base;
        if (rem == 1) {                 /* base of residue 0 */
            while (curr_k0 < k) {
                block_concat0 = block_concat0 * 10000 + 5247;
                pow10_block_len0 = pow10_block_len0 * 10000;
                curr_k0++;
            }
            seq_t shift = pow10_block_len0 * 10000;
            seq_t part1 = 147 * shift;
            seq_t part2 = block_concat0 * 10000;
            base = part1 + part2 + 5244;
        }
        else {                          /* rem == 3: base of residue 2 */
            while (curr_k2 < k) {
                block_concat2 = block_concat2 * 10000 + 5247;
                pow10_block_len2 = pow10_block_len2 * 10000;
                curr_k2++;
            }
            seq_t shift = pow10_block_len2 * 1000;
            seq_t part1 = 161247 * shift;
            seq_t part2 = block_concat2 * 1000;
            base = part1 + part2 + 384;
        }
        val = reverse(base);
    }

    return val;
}
