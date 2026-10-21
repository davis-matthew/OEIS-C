/*
 * A286846  Zeroless pandigital (9-digit) numbers where the first three digits
 *          minus the middle three digits equals the last three digits.
 * https://oeis.org/A286846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = nth 9‑digit pandigital number (digits 1‑9 each exactly
 *            once) where the first three digits minus the middle three digits
 *            equals the last three digits
 * Range:     u8 none, u16 none, u32 n <= 336, u64 n <= 336, u128 n <= 336,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *valid = NULL;
    static size_t valid_cnt = 0;
    static bool prepped = false;

    if (!prepped) {
        /* allocate space for the maximum number of permutations (9! = 362880) */
        size_t capacity = 362880;
#ifdef __cplusplus
        valid = new seq_t[capacity];
#else
        valid = (seq_t*)malloc(capacity * sizeof(seq_t));
#endif
        /* generate all permutations of digits 1‑9 in lexicographic order */
        int d[9] = {1,2,3,4,5,6,7,8,9};
        bool finished = false;
        size_t pos = 0;
        while (!finished) {
            /* compute the three triples */
            seq_t abc = (seq_t)d[0]*100 + (seq_t)d[1]*10 + d[2];
            seq_t def = (seq_t)d[3]*100 + (seq_t)d[4]*10 + d[5];
            if (abc >= def) {
                seq_t diff = abc - def;
                if (diff < 1000) {
                    seq_t ghi = (seq_t)d[6]*100 + (seq_t)d[7]*10 + d[8];
                    if (diff == ghi) {
                        /* build the full 9‑digit number */
                        seq_t val = 0;
                        for (int k = 0; k < 9; ++k) {
                            val = val*10 + d[k];
                        }
                        valid[pos++] = val;
                    }
                }
            }

            /* next_permutation algorithm */
            int i = 7;
            while (i >= 0 && d[i] >= d[i+1]) i--;
            if (i < 0) {
                finished = true;
            } else {
                int j = 8;
                while (d[j] <= d[i]) j--;
                int tmp = d[i]; d[i] = d[j]; d[j] = tmp;
                int l = i+1, r = 8;
                while (l < r) {
                    tmp = d[l]; d[l] = d[r]; d[r] = tmp;
                    l++; r--;
                }
            }
        }
        valid_cnt = pos;
        prepped = true;
    }

    if (n == 0 || n > valid_cnt) return 0;
    return valid[n-1];
}
