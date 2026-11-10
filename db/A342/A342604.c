/*
 * A342604  a(n) = Sum_{j=1..n} A003718(j-1)*prime(j).
 * https://oeis.org/A342604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} A007318(j-1)*prime(j), where A007318 is
 *            Pascal's triangle read by rows (the A003718 in the name is a
 *            typo for A007318, as the listed terms show: multipliers
 *            1;1,1;1,2,1;1,3,3,1;...). Computed with running prefix sums,
 *            building Pascal rows by addition
 * Range:     u8 n <= 11, u16 n <= 42, u32 n <= 242, u64 n <= 1301,
 *            u128 n <= 6376, big any n
 * Validated: standard, pass, 66 terms through n = 9874 (2026-11-10)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static seq_t *S = NULL;      /* S[m] = a(m) */
static idx_t S_cap = 0;
static idx_t S_have = 0;     /* number of terms computed */
static seq_t *prow = NULL;   /* current Pascal row r */
static idx_t prow_cap = 0;
static idx_t pr_r = 0;       /* current row index */
static idx_t pr_k = 0;       /* next position within row */
static uint64_t cur_p = 1;   /* last prime used */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (S == NULL) {
        S_cap = 64;
        S = mem_new(seq_t, (size_t)S_cap);
        S[0] = 0;
        prow_cap = 64;
        prow = mem_new(seq_t, (size_t)prow_cap);
        prow[0] = 1;
        pr_r = 0;
        pr_k = 0;
        S_have = 0;
        cur_p = 1;
    }
    if (n + 1 > S_cap) {
        idx_t nc = S_cap * 2;
        if (nc < n + 1) nc = n + 1;
        S = mem_resize(seq_t, S, (size_t)nc);
        S_cap = nc;
    }
    while (S_have < n) {
        uint64_t q;
        seq_t term;
        if (pr_k > pr_r) {
            idx_t i;
            pr_r++;
            if (pr_r + 1 > prow_cap) {
                idx_t nc = prow_cap * 2;
                if (nc < pr_r + 1) nc = pr_r + 1;
                prow = mem_resize(seq_t, prow, (size_t)nc);
                prow_cap = nc;
            }
            prow[pr_r] = 1;
            for (i = pr_r - 1; i >= 1; i--)
                prow[i] += prow[i - 1];
            pr_k = 0;
        }
        q = cur_p + 1;
        while (!is_prime_u64(q)) q++;
        cur_p = q;
        term = prow[pr_k] * (seq_t)q;
        S[S_have + 1] = S[S_have] + term;
        S_have++;
        pr_k++;
    }
    return S[n];
}
