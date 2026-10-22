/*
 * A380106  a(1) = 0; for n >= 1, if there exists an m < n such that a(m) =
 *          a(n), take the largest such m and let a(n+1) be the number of runs
 *          in the subsequence a(m)..a(n-1). Otherwise, a(n+1) = 0.
 * https://oeis.org/A380106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 417, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0.
   For n≥1 let m be the largest index < n with a(m)=a(n).
   If such m exists then a(n+1) = 1 + Σ_{k=m+1}^{n-1} δ(a(k),a(k+1))
   (i.e. the number of runs in the subsequence a(m)..a(n‑1));
   otherwise a(n+1)=0.
   δ(x,y)=1 if x≠y, else 0.
   The algorithm keeps the sequence and a prefix sum of δ. */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N 20000        /* sufficient for all test cases (n≤10000) */

static seq_t a_seq[MAX_N];          /* a(1)...a(MAX_N)  */
static idx_t diff_arr[MAX_N];       /* diff_arr[i] = 1 if a(i+1)≠a(i) else 0 */
static idx_t pref_arr[MAX_N];       /* prefix sums of diff_arr */
static idx_t len = 0;               /* number of terms already computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1, but safe guard */

    for (idx_t j = len + 1; j <= n; ++j) {  /* j is the 1‑based index to fill */
        if (j == 1) {                       /* a(1) = 0 */
            a_seq[0]     = 0;
            diff_arr[0]  = 0;
            pref_arr[0]  = 0;
        } else if (j == 2) {                 /* a(2) = 0 */
            a_seq[1]     = 0;
            diff_arr[1]  = 0;                /* a(2)==a(1) */
            pref_arr[1]  = pref_arr[0] + diff_arr[1];
        } else {                             /* j ≥ 3 */
            seq_t prev_val = a_seq[j - 2];   /* a(j‑1) */
            idx_t search_end = j - 3;        /* last position to search (≤ n‑2) */

            bool found = false;
            idx_t m_pos = 0;
            for (idx_t pos = search_end; ; --pos) {
                if (a_seq[pos] == prev_val) { found = true; m_pos = pos; break; }
                if (pos == 0) break;           /* reached the beginning */
            }

            seq_t new_val;
            if (!found) {
                new_val = 0;
            } else {
                /* number of runs in a(m)..a(n‑1):
                   1 + Σ diff_arr[m]..diff_arr[n‑2] == 1 + (pref[n‑2] - pref[m‑1]) */
                new_val = (seq_t)(1 + (pref_arr[search_end] - pref_arr[m_pos]));
            }

            a_seq[j - 1] = new_val;
            diff_arr[j - 1] = (a_seq[j - 1] != a_seq[j - 2]) ? 1 : 0;
            pref_arr[j - 1] = pref_arr[j - 2] + diff_arr[j - 1];
        }
        len = j; /* update number of computed terms */
    }

    return a_seq[n - 1];
}
