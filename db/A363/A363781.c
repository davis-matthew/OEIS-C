/*
 * A363781  a(n) = [x^n] 1/(Sum_{k>=0} x^(k^3))^n.
 * https://oeis.org/A363781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in 1/( Σ_{k≥0} x^(k^3) )^n Recurrence:
 *            f_0(0)=1, f_0(m)=0 for m>0 f_n(0)=1 f_n(m) = f_{n-1}(m) –
 *            Σ_{c∈C^+, c≤m} f_n(m–c), where C^+ = {1,8,27,…}. a(n)=f_n(n)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* static helper tables --------------------------------------------------- */
static idx_t *cube_vals = NULL;
static size_t cube_cnt = 0;

static void ensure_cubes(size_t nmax)
{
    size_t needed = 0;
    for (uint64_t k = 0; ; ++k) {
        uint64_t c = k * k * k;
        if (c > nmax) break;
        ++needed;
    }
    if (needed > cube_cnt) {
        idx_t *p = (idx_t *)realloc(cube_vals, needed * sizeof(idx_t));
        if (!p) abort();
        cube_vals = p;
        cube_cnt = needed;
        for (uint64_t k = 0; ; ++k) {
            uint64_t c = k * k * k;
            if (c > (uint64_t)nmax) break;
            cube_vals[k] = (idx_t)c;
        }
    }
}

/* cache for answers ------------------------------------------------------- */
static seq_t *answer_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_max = 0;

/* rows for dynamic recurrence -------------------------------------------- */
static seq_t *row_prev = NULL;
static seq_t *row_curr = NULL;
static size_t row_cap = 0;

/* ensure all values up to nmax are computed -------------------------------- */
static void ensure_cache(idx_t nmax)
{
    size_t N = (size_t)nmax;

    /* we still need to compute base case when nothing allocated yet */
    if (N <= cache_max && cache_cap > 0) return;

    /* enlarge answer cache if necessary */
    if (N + 1 > cache_cap) {
        seq_t *p = (seq_t *)realloc(answer_cache, (N + 1) * sizeof(seq_t));
        if (!p) abort();
        answer_cache = p;
        cache_cap = N + 1;
    }

    /* enlarge working rows if necessary */
    if (N + 1 > row_cap) {
        seq_t *p1 = (seq_t *)realloc(row_prev, (N + 1) * sizeof(seq_t));
        seq_t *p2 = (seq_t *)realloc(row_curr, (N + 1) * sizeof(seq_t));
        if (!p1 || !p2) abort();
        row_prev = p1;
        row_curr = p2;
        row_cap = N + 1;
    }

    /* ensure cube list up to N */
    ensure_cubes(N);

    /* base row: n = 0 */
    for (size_t i = 0; i <= N; ++i) row_prev[i] = (seq_t)0;
    row_prev[0] = (seq_t)1;
    answer_cache[0] = (seq_t)1;

    /* compute rows for n = 1 .. N */
    for (size_t n = 1; n <= N; ++n) {
        row_curr[0] = (seq_t)1;
        for (size_t k = 1; k <= N; ++k) {
            seq_t val = row_prev[k];              /* f_{n-1}(k)            */
            for (size_t ci = 1; ci < cube_cnt; ++ci) {
                idx_t c = cube_vals[ci];
                if (c > k) break;
                val -= row_curr[k - c];
            }
            row_curr[k] = val;
        }
        answer_cache[n] = row_curr[n];
        seq_t *tmp = row_prev;
        row_prev = row_curr;
        row_curr = tmp;
    }

    cache_max = N;
}

/* public function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return answer_cache[(size_t)n];
}
