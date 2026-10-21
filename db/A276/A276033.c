/*
 * A276033  Number of generalizations of the partition 1^n with all elements
 *          taken modulo 2.
 * https://oeis.org/A276033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k-1) = Σ_{i=1}^{k} C_i C_i = Catalan number a(2k) =
 *            Σ_{i=1}^{k+1} C_i – C_k (C_1=1, C_2=2, C_3=5, …)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

/* external helper */
extern seq_t catalan(idx_t k);

/* static storage for Catalan numbers and their prefix sums   */
/* cats[i]  = C_i   (i ≥ 0), sums[i] = Σ_{j=1}^{i} C_j     */
static seq_t *cats  = NULL;
static seq_t *sums  = NULL;
static idx_t  current_k = 0;   /* largest i for which cats[i] is valid    */
static size_t array_cap  = 0;   /* allocated array capacity (≥ current_k+1) */

/* ensure that cats[0..k_needed] and sums[0..k_needed] are computed */
static void compute_up_to(idx_t k_needed)
{
    /* allocate on first entry */
    if (cats == NULL)
    {
        array_cap = k_needed + 2;          /* +2 to avoid repeated re‑alloc */
        cats  = (seq_t*)malloc(sizeof(seq_t) * array_cap);
        sums  = (seq_t*)malloc(sizeof(seq_t) * array_cap);
        if (cats == NULL || sums == NULL) return;   /* allocation failure */

#ifdef __cplusplus
        /* construct the objects (C++ only) */
        for (size_t i = 0; i < array_cap; ++i)
        {
            new(&cats[i]) seq_t();
            new(&sums[i]) seq_t();
        }
#endif
        cats[0] = (seq_t)1;   /* C_0 = 1, unused but needed for recurrence */
        sums[0] = (seq_t)0;   /* Σ_{j=1}^{0} C_j = 0 */
        current_k = 0;
    }

    /* grow the arrays if necessary */
    if ((size_t)(k_needed + 1) > array_cap)
    {
        size_t new_cap = k_needed + 2;
        cats = (seq_t*)realloc(cats, sizeof(seq_t) * new_cap);
        sums = (seq_t*)realloc(sums, sizeof(seq_t) * new_cap);
        if (cats == NULL || sums == NULL) return;   /* allocation failure */
#ifdef __cplusplus
        /* construct new elements (C++ only) */
        for (size_t i = array_cap; i < new_cap; ++i)
        {
            new(&cats[i]) seq_t();
            new(&sums[i]) seq_t();
        }
#endif
        array_cap = new_cap;
    }

    /* compute new Catalan numbers and prefix sums */
    for (idx_t i = current_k + 1; i <= k_needed; ++i)
    {
        idx_t prev = i - 1;                          /* previous index   */
        /* recurrence: C_i = C_{i-1} * 2*(2*prev+1) / (prev+2) */
        seq_t num = cats[prev];
        num = num * (seq_t)2 * (seq_t)(2 * prev + 1);
        seq_t den = (seq_t)(prev + 2);
        cats[i] = num / den;

        if (i == 1)
            sums[i] = cats[i];
        else
            sums[i] = sums[i - 1] + cats[i];
    }
    current_k = k_needed;
}

/* the sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* not used – protects against underflow */

    /* need Catalan numbers up to floor(n/2)+1 */
    idx_t k_needed = (n >> 1) + 1;   /* ceil((n+1)/2) */
    compute_up_to(k_needed);

    if (n & 1)                         /* odd n = 2k-1 */
    {
        idx_t k = (n + 1) >> 1;        /* = (n+1)/2 */
        return sums[k];
    }
    else                               /* even n = 2k   */
    {
        idx_t k = n >> 1;              /* = n/2         */
        return sums[k + 1] - cats[k];
    }
}
