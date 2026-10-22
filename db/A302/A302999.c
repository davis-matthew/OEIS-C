/*
 * A302999  a(n) = Product_{k=1..n} (Fibonacci(k+2) - 1).
 * https://oeis.org/A302999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (Fibonacci(k+2) - 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*              = Product_{k=1..n} Sum_{j=1..k} Fibonacci(j)   */
/*              = Product_{k=1..n} A000071(k+2).                 */
#include <stdlib.h>

#ifdef __cplusplus
/* dynamic allocation helpers for C++: use new/delete for proper object construction */
static void* alloc_seq(size_t n) { return static_cast<void*>(new seq_t[n]); }
static void free_seq(void* p) { delete [] static_cast<seq_t*>(p); }
static void* realloc_seq(void* p, size_t old_n, size_t new_n)
{
    seq_t* old = static_cast<seq_t*>(p);
    seq_t* neu = new seq_t[new_n];
    for (size_t i = 0; i < old_n; ++i) neu[i] = old[i];
    delete [] old;
    return neu;
}
#else
/* dynamic allocation helpers for C: use malloc/realloc/free */
static void* alloc_seq(size_t n) { return malloc(n * sizeof(seq_t)); }
static void free_seq(void* p)    { free(p); }
static void* realloc_seq(void* p, size_t old_n, size_t new_n)
{
    (void)old_n; /* unused but keep signature consistent */
    return realloc(p, new_n * sizeof(seq_t));
}
#endif

seq_t a(idx_t n)
{
    /* static state that remembers the largest n computed so far */
    static idx_t last_n = 0;          /* largest n for which prod[.] is valid */
    static size_t capacity = 0;       /* current allocated array size */
    static seq_t *prod = NULL;        /* product array: prod[i] = a(i)   */
    static seq_t *sum  = NULL;        /* running sums: sum[i] = \sum_{j=1}^i F(j) */
    static seq_t f_prev = 0;          /* F(k-1) during extension          */
    static seq_t f_curr = 1;          /* F(k)   during extension          */
    static int initialized = 0;

    /* one‑time initialization */
    if (!initialized) {
        capacity = 1;                 /* store index 0 */
        prod = (seq_t*)alloc_seq(capacity);
        sum  = (seq_t*)alloc_seq(capacity);
        prod[0] = (seq_t)1;          /* a(0) = 1 (empty product)         */
        sum[0]  = (seq_t)0;          /* sum of zero Fibonacci numbers     */
        initialized = 1;
    }

    /* extend internal arrays to include n */
    if ((idx_t)capacity <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_prod = (seq_t*)realloc_seq(prod, capacity, new_cap);
        seq_t *new_sum  = (seq_t*)realloc_seq(sum,  capacity, new_cap);
        prod = new_prod;
        sum  = new_sum;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = last_n + 1; k <= n; ++k) {
        /* fib sequence: F(k) is f_curr, next is f_prev + f_curr */
        seq_t Fk = f_curr;                       /* F(k)  */
        seq_t Fk1 = f_prev + f_curr;             /* F(k+1)*/

        /* running sum of Fibonacci numbers up to k */
        seq_t sum_k = sum[k-1] + Fk;

        /* product of sums up to k */
        seq_t prod_k = prod[k-1] * sum_k;

        /* store results */
        sum[k]  = sum_k;
        prod[k] = prod_k;

        /* advance fibonacci iterators for next k */
        f_prev = Fk;
        f_curr = Fk1;
    }

    last_n = n;
    return prod[n];
}
