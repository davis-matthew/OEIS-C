/*
 * A293209  Let b be the lexicographically earliest sequence of positive terms
 *          such that the function f defined by f(n) = Sum_{k=1..n} (i^k *
 *          b(k)) for any n >= 0 is injective (where i denotes the imaginary
 *          unit), and b(n) != b(n+1) and b() != b(n+2) for any n > 0; a(n) =
 *          imaginary part of f(n).
 * https://oeis.org/A293209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = imaginary part of f(n) where f(n) = Sum_{k=1..n} (i^k *
 *            b(k)), and b(k) is the lexicographically earliest positive
 *            sequence with f(n) injective and b(k) != b(k+1) and b(k) !=
 *            b(k+2)
 * Range:     i8 n <= 9228, i16 n <= 9228, i32 n <= 9228, i64 n <= 9228,
 *            i128 n <= 9228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*----- data structures -----*/
typedef struct { seq_t r; seq_t i; } Pair;

/*----- global buffers -----*/
static seq_t *b_arr   = NULL;       /* b(k)            */
static seq_t *R_arr   = NULL;       /* real parts     */
static seq_t *I_arr   = NULL;       /* imaginary parts*/
static Pair *vis_arr = NULL;        /* visited points */

static idx_t  current_len = 0;      /* highest n that has been built */
static size_t visited_sz  = 0;      /* number of visited points     */
static idx_t  target_len  = 0;      /* length we are currently building */

/* maximum capacity – safe for n up to about 10 000 */
#define MAX_CAP 20000

/*----- helper: recursive construction -----*/
static bool build_rec(idx_t k)
{
    if (k > target_len) return true;   /* finished */

    seq_t prevR = R_arr[k-1];
    seq_t prevI = I_arr[k-1];

    for (int cand = 1; ; ++cand) {
        seq_t cand_ts = (seq_t)cand;

        /* b must not equal the two previous values */
        if (k >= 1 && cand_ts == b_arr[k-1]) continue;
        if (k >= 2 && cand_ts == b_arr[k-2]) continue;

        /* new coordinates */
        seq_t newR = prevR;
        seq_t newI = prevI;

        idx_t m = k & 3;                 /* k mod 4 */
        switch (m) {
            case 1: newI = prevI + cand_ts;  break;  /* i^1  */
            case 3: newI = prevI - cand_ts;  break;  /* i^3  */
            case 2: newR = prevR - cand_ts;  break;  /* i^2  */
            case 0: newR = prevR + cand_ts;  break;  /* i^0  */
        }

        /* must not repeat a previous point */
        bool dup = false;
        for (size_t v = 0; v < visited_sz; ++v) {
            if (vis_arr[v].r == newR && vis_arr[v].i == newI) {
                dup = true; break;
            }
        }
        if (dup) continue;

        /* accept this candidate */
        b_arr[k]   = cand_ts;
        R_arr[k]   = newR;
        I_arr[k]   = newI;
        vis_arr[visited_sz].r = newR;
        vis_arr[visited_sz].i = newI;
        visited_sz++;

        if (build_rec(k+1)) return true;  /* success */

        /* backtrack */
        visited_sz--;
    }
}

/*----- public function -----*/
seq_t a(idx_t n)
{
    static bool initialized = false;
    const size_t CAP = MAX_CAP;

    if (!initialized) {
        /* allocate buffers */
        b_arr   = (seq_t*)malloc((CAP+1) * sizeof(seq_t));
        R_arr   = (seq_t*)malloc((CAP+1) * sizeof(seq_t));
        I_arr   = (seq_t*)malloc((CAP+1) * sizeof(seq_t));
        vis_arr = (Pair*) malloc((CAP+1) * sizeof(Pair));

        /* initial state: f(0) = 0 */
        b_arr[0] = (seq_t)0;
        R_arr[0] = (seq_t)0;
        I_arr[0] = (seq_t)0;
        vis_arr[0].r = (seq_t)0;
        vis_arr[0].i = (seq_t)0;
        visited_sz = 1;
        current_len = 0;
        initialized = true;
    }

    if (n > current_len) {
        /* build up to n (if needed) */
        target_len = n;
        if (!build_rec(current_len + 1)) {
            /* should never happen */
            return (seq_t)0;
        }
        current_len = n;
    }

    /* a(0)=0, a(1)=I[1], etc. */
    return I_arr[n];
}
