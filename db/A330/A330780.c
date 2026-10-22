/*
 * A330780  Lexicographically earliest sequence of positive integers such that
 *          for any v > 0, the value v appears up to v^2 times, and the
 *          associate function f defined by f(n) = Sum_{k = 1..n} a(k) * m(k)
 *          for n >= 0 is injective (where {m(k)} corresponds to knight's
 *          moves, see Comments for precise definition).
 * https://oeis.org/A330780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A330780: Lexicographically earliest sequence of positive integers
 *  such that each value v appears at most v^2 times and the
 *  cumulative sum f(n) = Σ_{k=1}^n a(k)*m(k) (m(k) is the
 *  8‑periodic knight‑move vector) is injective.
 *
 *  Implementation: greedy construction – for each position n,
 *  try the smallest admissible value until all conditions are
 *  satisfied.  All state is kept in static arrays that grow only
 *  as needed.  The knight moves are stored as signed 64‑bit
 *  integers to keep the arithmetic correct. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*  Knight‑move offsets (real and imaginary parts) as signed 64‑bit. */
static const int64_t m_re[8] = {  2,  1, -1, -2, -2, -1,  1,  2 };
static const int64_t m_im[8] = {  1,  2,  2,  1, -1, -2, -2, -1 };

/*  State storage. */
static idx_t  computed      = 0;          /* number of terms known */
static seq_t *terms         = NULL;        /* a[0 .. computed-1]     */
static int64_t *sum_re      = NULL;        /* partial real sums      */
static int64_t *sum_im      = NULL;        /* partial imag sums      */
static idx_t *counts       = NULL;         /* counts[v] = occurrences */
static size_t  terms_cap     = 0;          /* allocated size of terms */
static size_t  sums_cap      = 0;          /* allocated size of sums  */
static size_t  counts_cap    = 0;          /* allocated size of counts */

/*------------------------------------------------------------*/
static void ensure_terms_cap(idx_t need)
{
    if (need <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}
static void ensure_sums_cap(idx_t need)
{
    if (need <= sums_cap) return;
    size_t new_cap = sums_cap ? sums_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    sum_re = (int64_t *)realloc(sum_re, new_cap * sizeof(int64_t));
    sum_im = (int64_t *)realloc(sum_im, new_cap * sizeof(int64_t));
    sums_cap = new_cap;
}
static void ensure_counts_cap(idx_t val)
{
    if (val < counts_cap) return;          /* index is 1‑based */
    size_t new_cap = counts_cap ? counts_cap << 1 : 64;
    while (new_cap <= (size_t)val) new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(counts, new_cap * sizeof(idx_t));
    /* zero the newly allocated portion */
    memset(tmp + counts_cap, 0, (new_cap - counts_cap) * sizeof(idx_t));
    counts = tmp;
    counts_cap = new_cap;
}
static void init_state(void)
{
    if (computed != 0) return;             /* already inited */
    ensure_terms_cap(64);
    ensure_sums_cap(65);                   /* sums need one extra slot */
    ensure_counts_cap(256);
    /* basis S(0) = (0,0) */
    sum_re[0] = 0;
    sum_im[0] = 0;
}

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset starts at 1 */
    init_state();

    while (computed < n)
    {
        idx_t  pos   = computed % 8;          /* current knight move index */
        int64_t cur_r = sum_re[computed];
        int64_t cur_i = sum_im[computed];

        /* search for smallest admissible value */
        for (idx_t val = 1; ; ++val)
        {
            ensure_counts_cap(val);
            if (counts[val] >= (idx_t)val * (idx_t)val) continue;  /* exceed limit */

            int64_t new_r = cur_r + (int64_t)val * m_re[pos];
            int64_t new_i = cur_i + (int64_t)val * m_im[pos];

            /* check that new sum is distinct from all previous sums */
            bool dup = false;
            for (idx_t i = 0; i < computed; ++i)
            {
                if (new_r == sum_re[i] && new_i == sum_im[i]) { dup = true; break; }
            }
            if (dup) continue;            /* try next value */

            /* commit this term */
            ensure_terms_cap(computed + 1);
            ensure_sums_cap(computed + 1);

            terms[computed]   = (seq_t)val;
            counts[val] += 1;
            sum_re[computed + 1] = new_r;
            sum_im[computed + 1] = new_i;

            ++computed;
            break;                       /* next position ready */
        }
    }
    return terms[n - 1];
}
