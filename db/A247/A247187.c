/*
 * A247187  Numbers containing nine distinct decimal digits (zero is not
 *          considered) and having the property that the digits 1 to 5 appear
 *          in the order but not the digits 1 to 6.
 * https://oeis.org/A247187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2520, u64 n <= 2520, u128 n <= 2520,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A247187.
   The sequence consists of all 9‑digit numbers containing the digits
   1–9 exactly once (zero is not allowed), such that digits 1 to 5
   appear in that relative order, but the subsequence 1–6 does not
   appear.  There are exactly 5·7·8·9 = 2520 such numbers.
   The function returns the n‑th number in this list (a(1) is the
   smallest such number).  The numbers are generated in sorted
   order without using a generic qsort that would be incompatible
   with arbitrary‑precision seq_t types. */

#include <stdlib.h>
#include <stddef.h>

static seq_t seq_precomp[2520];
static int seq_built = 0;

/* Simple insertion sort that works with any seq_t that supports
   the comparison operators < and >.  No casting of `void*` is
   required, so the code is C/C++ compatible. */
static void sort_seq(void)
{
    for (size_t i = 0; i < 2520 - 1; ++i) {
        for (size_t j = i + 1; j < 2520; ++j) {
            if (seq_precomp[j] < seq_precomp[i]) {
                seq_t tmp = seq_precomp[i];
                seq_precomp[i] = seq_precomp[j];
                seq_precomp[j] = tmp;
            }
        }
    }
}

static void build_seq(void)
{
    const int base[5] = {1, 2, 3, 4, 5};
    int idx = 0;

    for (int pos6 = 0; pos6 < 5; ++pos6) {
        int seq5[6];
        int p = 0;
        for (int i = 0; i < 5; ++i) {
            if (p == pos6) seq5[p++] = 6;
            seq5[p++] = base[i];
        }

        for (int pos7 = 0; pos7 <= 6; ++pos7) {
            int tmp7[7];
            int q = 0;
            for (int i = 0; i < 7; ++i) {
                if (i == pos7) tmp7[i] = 7;
                else          tmp7[i] = seq5[q++];
            }

            for (int pos8 = 0; pos8 <= 7; ++pos8) {
                int tmp8[8];
                q = 0;
                for (int i = 0; i < 8; ++i) {
                    if (i == pos8) tmp8[i] = 8;
                    else           tmp8[i] = tmp7[q++];
                }

                for (int pos9 = 0; pos9 <= 8; ++pos9) {
                    int tmp9[9];
                    q = 0;
                    for (int i = 0; i < 9; ++i) {
                        if (i == pos9) tmp9[i] = 9;
                        else           tmp9[i] = tmp8[q++];
                    }

                    /* Convert the array of digits to the integer value. */
                    seq_t val = 0;
                    for (int d = 0; d < 9; ++d)
                        val = val * 10 + (seq_t)tmp9[d];

                    seq_precomp[idx++] = val;
                }
            }
        }
    }

    sort_seq();
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (!seq_built) {
        build_seq();
        seq_built = 1;
    }
    if (n == 0 || n > 2520) return 0;
    return seq_precomp[n - 1];
}
