/*
 * A362065  Write the English name of a(n); sum the ranks of its letters in
 *          the alphabet; divide the sum by a(n); the result is an integer.
 * https://oeis.org/A362065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full, word
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the nth integer such that the sum of the
   alphabetical ranks of the letters of its English name
   is divisible by that integer. */

/* Include required headers */
#include <stdlib.h>
#include <stdbool.h>

/* ----- Letter sum tables ----- */

/* Sum of letter ranks for numbers 0 … 19 (0 unused). */
static const seq_t unitSum[20] = {
    104, /* ZERO (unused) */
    34,  /* ONE   */
    58,  /* TWO   */
    56,  /* THREE */
    60,  /* FOUR  */
    42,  /* FIVE  */
    52,  /* SIX   */
    65,  /* SEVEN */
    49,  /* EIGHT */          /* fixed: 49, not 61 */
    42,  /* NINE  */
    39,  /* TEN   */
    63,  /* ELEVEN */
    87,  /* TWELVE */
    99,  /* THIRTEEN */
    104, /* FOURTEEN */
    65,  /* FIFTEEN */
    96,  /* SIXTEEN */
    109, /* SEVENTEEN */
    73,  /* EIGHTEEN */
    86   /* NINETEEN */
};

/* Sum of letter ranks for multiples of ten 0 … 90 (except 0,10). */
static const seq_t tensSum[10] = {
    0,    /* 0 */
    0,    /* 1 */
    107,  /* TWENTY */
    100,  /* THIRTY */
    84,   /* FOURTY */
    66,   /* FIFTY */
    97,   /* SIXTY */
    110,  /* SEVENTY */
    74,   /* EIGHTY */
    87    /* NINETY */
};

static const seq_t hundredSum = 74; /* HUNDRED */

/* Sum of letter ranks for scale words: thousand, million, billion, trillion. */
static const seq_t scaleSum[6] = {
    0,    /* 0 */
    116,  /* THOUSAND */
    84,   /* MILLION  */
    73,   /* BILLION  */
    109,  /* TRILLION */
    0     /* beyond (not required for current bounds) */
};

/* ----- Helper: sum of letter ranks of an integer n ----- */
static seq_t letter_sum(idx_t n)
{
    if (n < 20)                /* units or teens */
        return unitSum[n];

    if (n < 100) {             /* tens */
        seq_t sum = tensSum[n / 10];
        idx_t r = n % 10;
        if (r)
            sum += unitSum[r];
        return sum;
    }

    if (n < 1000) {            /* hundreds */
        idx_t h = n / 100;
        seq_t sum = unitSum[h] + hundredSum;
        idx_t r = n % 100;
        if (r)
            sum += letter_sum(r);
        return sum;

    }

    /* Numbers ≥ 1000: split into highest scale and remainder. */
    idx_t power = 1000;
    size_t idxScale = 1;
    /* loop while n has a higher scale word */
    while (power <= n / 1000) {
        power *= 1000;
        ++idxScale;
    }

    idx_t high = n / power;
    seq_t sum = letter_sum(high) + scaleSum[idxScale];
    idx_t rem = n % power;
    if (rem)
        sum += letter_sum(rem);
    return sum;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *terms = NULL;
    static size_t cap   = 0;
    static size_t count = 0;
    static idx_t next   = 1;          /* next integer to test */

    /* Ensure cache capacity */
    if (cap < count + 1) {
        size_t newcap = cap ? cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms = tmp;
        cap   = newcap;
    }

    /* Return cached value if available */
    if (n <= count) {
        return terms[n - 1];
    }

    /* Generate terms until we reach the requested index */
    while (count < n) {
        if (letter_sum(next) % (seq_t)next == 0) {
            terms[count++] = (seq_t)next;
        }
        ++next;
    }

    return terms[n - 1];
}
