/*
 * A380247  Triangle read by rows: T(n,k) is the number of the k-th eliminated
 *          person in the variation of the Josephus elimination process for n
 *          people, where the number of people skipped correspond to the
 *          number of letters in the next number in English alphabet.
 * https://oeis.org/A380247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, tabl
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Validated: partial, pass, 106 terms through n = 106 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by simulation of the Josephus variation with skip lengths equal to the number of letters in the English words of successive natural numbers. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return the number of letters in the English word for a given positive integer x. */
static uint64_t wordlen_uint64(uint64_t x)
{
    /* One and the tens up to 19 */
    static const uint8_t ones[20] = {
        0, /* unused */
        3, /* one */
        3, /* two */
        5, /* three */
        4, /* four */
        4, /* five */
        3, /* six */
        5, /* seven */
        5, /* eight */
        4, /* nine */
        3, /* ten */
        6, /* eleven */
        6, /* twelve */
        8, /* thirteen */
        8, /* fourteen */
        7, /* fifteen */
        7, /* sixteen */
        9, /* seventeen */
        8, /* eighteen */
        8  /* nineteen */
    };
    static const uint8_t tens[10] = {
        0, /* unused */
        0, /* unused */
        6, /* twenty */
        6, /* thirty */
        5, /* forty */
        5, /* fifty */
        5, /* sixty */
        7, /* seventy */
        6, /* eighty */
        6  /* ninety */
    };

    uint64_t len = 0;

    /* Handle thousands and beyond */
    if (x >= 1000) {
        uint64_t thousands = x / 1000;
        len += wordlen_uint64(thousands); /* recursively compute */
        len += 8;                         /* length of "thousand" */
        x %= 1000;
        if (x > 0) {
            len += 3; /* "and" */
        }
    }

    /* Handle hundreds */
    if (x >= 100) {
        uint64_t hundreds = x / 100;
        len += ones[hundreds]; /* one, two, ... */
        len += 7;             /* "hundred" */
        x %= 100;
        if (x > 0) {
            len += 3;         /* "and" */
        }
    }

    /* Handle tens and ones */
    if (x >= 20) {
        uint64_t t = x / 10;
        len += tens[t];
        x = x % 10;
        if (x > 0) {
            len += ones[x];
        }
    } else if (x > 0) {
        len += ones[x];
    }

    return len;
}

/* seq_t and idx_t are defined by the build system. */
extern idx_t _; /* placeholder only */

/* Global buffer for the concatenated output sequence */
static seq_t *global_terms = NULL;
static size_t global_cap   = 0;
static idx_t  global_size  = 0;
static idx_t  next_row     = 1;            /* next row to be generated */

/* Append a value to the global sequence buffer. */
static void global_append(seq_t val)
{
    if (global_size == global_cap) {
        size_t newcap = (global_cap == 0) ? 16 : global_cap * 2;
        global_terms = mem_resize(seq_t, global_terms, newcap);
        global_cap   = newcap;
    }
    global_terms[global_size++] = val;
}

/* Compute the elimination order for a circle of size n and append to the global buffer. */
static void compute_row(idx_t n)
{
    int *next  = (int*)malloc((n + 1) * sizeof(int));
    int *prev  = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= (int)n; ++i) {
        next[i] = (i == (int)n) ? 1 : (i + 1);
        prev[i] = (i == 1) ? (int)n : (i - 1);
    }

    int cur = 1;  /* starting pointer */

    for (idx_t step = 1; step <= n; ++step) {
        uint64_t skip = wordlen_uint64(step);
        for (uint64_t s = 0; s < skip; ++s) {
            cur = next[cur];
        }

        int elim = cur;
        global_append((seq_t)elim);

        /* Remove elim from the circle */
        int prev_elim = prev[elim];
        int next_elim = next[elim];
        next[prev_elim] = next_elim;
        prev[next_elim] = prev_elim;

        cur = next_elim; /* move to the next survivor */
    }

    free(next);
    free(prev);
}

/* Required function definition */
seq_t a(idx_t n)
{
    while (global_size < n) {
        compute_row(next_row);
        ++next_row;
    }
    return global_terms[n - 1];
}
