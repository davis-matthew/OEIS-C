/*
 * A287657  {0->01, 1->10}-transform of the infinite Fibonacci word A003849.
 * https://oeis.org/A287657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = bit((n+1)/2) when n is odd,
    a(n) = 1 - bit(n/2)   when n is even,
    where bit(k) gives the k‑th (1‑based) symbol of the infinite Fibonacci word
    0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,... */
#include <stdlib.h>

static idx_t *fib_len = NULL;   /* Fibonacci word lengths: 1,2,3,5,8,... */
static size_t fib_cap = 0;
static size_t fib_sz  = 0;

/* Ensure fib_len[] contains lengths > need */
static void _ensure_fib(idx_t need)
{
    if (!fib_len) {
        fib_cap = 8;
        fib_len = (idx_t*)malloc(fib_cap * sizeof(idx_t));
        if (!fib_len) return;          /* allocation failed – abort */
        fib_len[0] = 1;            /* F_0 = "0"   */
        fib_len[1] = 2;            /* F_1 = "01"  */
        fib_sz  = 2;
    }
    while (fib_len[fib_sz - 1] <= need) {
        if (fib_sz == fib_cap) {
            fib_cap <<= 1;
            idx_t *tmp = (idx_t*)realloc(fib_len, fib_cap * sizeof(idx_t));
            if (!tmp) return;           /* allocation failed – abort */
            fib_len = tmp;
        }
        fib_len[fib_sz] = fib_len[fib_sz - 1] + fib_len[fib_sz - 2];
        fib_sz++;
    }
}

/* Return the 0‑based letter (0 or 1) at position pos (1‑based) of the Fibonacci word. */
static seq_t _fib_word_bit(idx_t pos)
{
    if (pos == 1) return 0;            /* F_0 = "0"   */
    if (pos == 2) return 1;            /* F_1 = "01"  */

    /* find smallest n with fib_len[n] >= pos */
    _ensure_fib(pos);

    /* iterative descent */
    while (pos > 2) {
        /* find k such that fib_len[k-1] < pos <= fib_len[k] */
        size_t k = 1;
        while (fib_len[k] < pos) ++k;
        if (pos > fib_len[k-1]) {
            pos -= fib_len[k-1];
            k -= 2;   /* jump to F_{k-2} */
        } else {
            k -= 1;   /* jump to F_{k-1} */
        }
    }

    /* Now pos is 1 or 2, and k is 0 or 1 */
    if (pos == 1) return 0;    /* F_0 or first symbol of F_1 */
    return 1;                  /* second symbol of F_1 */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* protect against offset misuse */

    /* n odd: take the first bit of the pair; n even: take its complement */
    if (n & 1U) {           /* odd */
        idx_t k = (n + 1) / 2;      /* 1‑based position in Fibonacci word */
        return _fib_word_bit(k);
    } else {                /* even */
        idx_t k = n / 2;
        return 1 - (seq_t)_fib_word_bit(k);
    }
}
