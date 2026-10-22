/*
 * A378889  G.f. A(x) satisfies A(x) = ( 1 + x*A(x)^(4/3)/(1 + x*A(x)^(1/3))
 *          )^3.
 * https://oeis.org/A378889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i+j+k=n} b_i * b_j * b_k where the auxiliary sequence
 *            b satisfies b_0 = 1, b_n = b_{n-1} + c4_{n-1} - c2_{n-1}, c2_n =
 *            Σ_{i=0..n} b_i * b_{n-i}, c4_n = Σ_{t=0..n} c2_t * c2_{n-t}
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The arrays b, c2, c4 and the final sequence a_arr are built incrementally
   and kept in static storage so that calls to a(idx_t n) may come in any order. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *b_arr   = NULL;
static seq_t *c2_arr  = NULL;
static seq_t *c4_arr  = NULL;
static seq_t *a_arr   = NULL;
static size_t cap     = 0;      /* allocated capacity */
static size_t max_n   = 0;      /* highest n that has been computed */

/* enlarge the storage to hold at least 'need' entries (indices 0..need-1)   */
static void grow_if_needed(size_t need)
{
    if (need <= cap)
        return;
    size_t new_cap = cap > 0 ? cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    b_arr   = (seq_t *)realloc(b_arr,   new_cap * sizeof(seq_t));
    c2_arr  = (seq_t *)realloc(c2_arr,  new_cap * sizeof(seq_t));
    c4_arr  = (seq_t *)realloc(c4_arr,  new_cap * sizeof(seq_t));
    a_arr   = (seq_t *)realloc(a_arr,   new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* initialise the first entry (n = 0) the first time the function is called   */
static void ensure_initial(void)
{
    if (max_n > 0)
        return;
    grow_if_needed(1);             /* capacity at least 1              */
    b_arr[0]   = 1;
    c2_arr[0]  = 1;
    c4_arr[0]  = 1;
    a_arr[0]   = 1;
    max_n = 0;
}

/* compute arrays up to index 'up_to' (inclusive)                              */
static void init_up_to(size_t up_to)
{
    if (up_to <= max_n)
        return;
    grow_if_needed(up_to + 1);   /* indices 0..up_to                */
    for (size_t m = max_n; m < up_to; ++m) {
        /* index of the new term */
        size_t n = m + 1;

        /* b[n] = b[m] + c4[m] - c2[m] */
        seq_t bn = b_arr[m];
        bn += c4_arr[m];
        bn -= c2_arr[m];
        b_arr[n] = bn;

        /* c2[n] = Σ_{i=0..n} b_i * b_{n-i} */
        seq_t sum_c2 = 0;
        for (size_t i = 0; i <= n; ++i)
            sum_c2 += b_arr[i] * b_arr[n - i];
        c2_arr[n] = sum_c2;

        /* c4[n] = Σ_{t=0..n} c2_t * c2_{n-t} */
        seq_t sum_c4 = 0;
        for (size_t t = 0; t <= n; ++t)
            sum_c4 += c2_arr[t] * c2_arr[n - t];
        c4_arr[n] = sum_c4;

        /* a[n] = Σ_{i=0..n} b_i * c2_{n-i}   (convolution of b with c2) */
        seq_t sum_a = 0;
        for (size_t i = 0; i <= n; ++i)
            sum_a += b_arr[i] * c2_arr[n - i];
        a_arr[n] = sum_a;
    }
    max_n = up_to;
}

/* public function requested by the problem statement */
seq_t a(idx_t n)
{
    /* ensure the static data structures are initialised   */
    ensure_initial();

    /* convert the idx_t argument to an ordinary size_t    */
    size_t target = (size_t)n;

    /* compute values up to the requested index if needed */
    if (target > max_n)
        init_up_to(target);

    /* return the requested term                      */
    return a_arr[target];
}
