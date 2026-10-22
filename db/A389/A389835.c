/*
 * A389835  Numbers k such that wt(Fibonacci(k)) * e > k, where wt(x) is the
 *          binary weight of x (A000120), and e is the natural logarithm base.
 * https://oeis.org/A389835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* An explicit search implementation for A389835.
   The sequence consists of the positive integers k for which
   weight(Fib(k)) * e > k, where weight(x) is the number of 1‑bits
   in the binary expansion of x and e is the natural base.  This
   implementation does not cache computed terms; instead for each
   call it generates Fibonacci numbers up to the point where the
   requested index appears.  Because the sequence terminates at
   k = 4366, the search is bounded by a few thousand iterations
   and easily fits within the time limit. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Simple arbitrary‑precision integer (little‑endian 64‑bit limbs)     */
/*  This is only needed for the Fibonacci recurrence.                 */
/* ------------------------------------------------------------------ */
typedef struct {
    uint64_t w[128];      /* enough for 128*64 = 8192 bits (> 4366th Fib) */
    size_t sz;            /* number of limbs used                               */
} BigInt;

static void big_zero(BigInt *b) { b->sz = 1; b->w[0] = 0; }
static void big_one (BigInt *b) { b->sz = 1; b->w[0] = 1; }

/* a + b  ->  res   (a, b may alias with res) */
static void big_add(const BigInt *a, const BigInt *b, BigInt *res)
{
    size_t n = (a->sz > b->sz) ? a->sz : b->sz;
    unsigned __int128 carry = 0;
    size_t i;
    for (i = 0; i < n; ++i) {
        unsigned __int128 s = carry;
        if (i < a->sz) s += a->w[i];
        if (i < b->sz) s += b->w[i];
        res->w[i] = (uint64_t)s;
        carry = s >> 64;
    }
    if (carry) {
        res->w[n] = (uint64_t)carry;
        res->sz = n + 1;
    } else
        res->sz = n;
}

/* popcount of a BigInt */
static unsigned int big_popcount(const BigInt *b)
{
    unsigned int c = 0;
    for (size_t i = 0; i < b->sz; ++i)
        c += __builtin_popcountll(b->w[i]);
    return c;
}

/* ------------------------------------------------------------------ */
/*  Constants for the inequality weight * e > k                     */
/*  e ≈ 2.71828182845904523536...                                    */
/*  We use e * 10^18 as an integer and compare  weight*e10  with  k*10^18  */
/* ------------------------------------------------------------------ */
static const uint64_t SCALE   = 1000000000000000000ULL;          /* 10^18 */
static const unsigned __int128 CEIL_E10 = (unsigned __int128)2718281828459045235ULL; /* e*10^18 */

/* ------------------------------------------------------------------ */
/*  Search for the n‑th element of the sequence                       */
/* ------------------------------------------------------------------ */
static const idx_t MAX_K = 5000;   /* covers the known endpoint 4366 */

/* a(n) returns the n‑th term of the sequence (indexing starts at 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    BigInt f_prev, f_curr, f_next;
    big_zero(&f_prev);          /* Fib(0) = 0 */
    big_one (&f_curr);          /* Fib(1) = 1 */

    idx_t count = 0;            /* number of terms found so far */

    for (idx_t k = 1; k <= MAX_K; ++k) {
        /* weight of Fib(k) */
        unsigned int w = big_popcount(&f_curr);

        /* test weight*e > k  using integer arithmetic  */
        unsigned __int128 left  = (unsigned __int128)w * CEIL_E10;
        unsigned __int128 right = (unsigned __int128)k * SCALE;
        if (left > right) {
            count++;
            if (count == n)
                return (seq_t)k;
        }

        /* advance Fibonacci: f_next = f_prev + f_curr */
        big_add(&f_prev, &f_curr, &f_next);
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* if n is too large the sequence has no such element */
    return 0;
}
