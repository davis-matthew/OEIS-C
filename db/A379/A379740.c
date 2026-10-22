/*
 * A379740  Number of calls to Karatsuba's multiplication algorithm K(x,y)
 *          when recursively calculating K(Fibonacci(n),Fibonacci(n+1)) in
 *          decimal digits.
 * https://oeis.org/A379740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of calls to Karatsuba's multiplication
 *            algorithm when recursively multiplying Fibonacci(n) and
 *            Fibonacci(n+1) in decimal digits. The count includes the initial
 *            call and every recursive call that occurs when both operands
 *            have at least two digits
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   strip_leading_zeroes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/strip_leading_zeroes.h"

/* Helper: duplicate a C string (used for the Fibonacci cache). */
static char *dup_str(const char *s)
{
    size_t len = strlen(s);
    char *d = (char *)malloc(len + 1);
    if (d) {
        memcpy(d, s, len + 1);
    }
    return d;
}

/* Helper: add two non‑negative decimal strings, result placed in 'out'.
   The output buffer must be large enough to hold the sum. */
#define MAX_SUM_LEN 2105   /* enough for Fibonacci(10000) + 1 digit */
static void add_str(const char *a, const char *b, char *out)
{
    size_t la = strlen(a);
    size_t lb = strlen(b);
    size_t i = la, j = lb;
    int carry = 0;
    size_t pos = 0;
    char tmp[MAX_SUM_LEN];

    while (i > 0 || j > 0 || carry) {
        int da = (i > 0) ? a[--i] - '0' : 0;
        int db = (j > 0) ? b[--j] - '0' : 0;
        int s  = da + db + carry;
        tmp[pos++] = (char)('0' + (s % 10));
        carry = s / 10;
    }

    /* reverse the result */
    for (size_t k = 0; k < pos; ++k) {
        out[k] = tmp[pos - 1 - k];
    }
    out[pos] = '\0';
}

/* Recursive routine that counts the number of Karatsuba calls
   for multiplying the two decimal numbers x and y. */
static seq_t rec(const char *x, size_t lx, const char *y, size_t ly)
{
    /* Stop if either operand has fewer than two digits. */
    if (lx < 2 || ly < 2) {
        return 1;
    }

    /* Determine the split point k = floor(max(lx,ly)/2). */
    size_t L = (lx > ly) ? lx : ly;
    size_t k = L / 2;

    /* Split x into high and low parts. */
    size_t len_lo_x = (lx <= k) ? lx : k;
    size_t len_hi_x = lx - len_lo_x;
    char hi_x[2105], lo_x[2105];
    if (len_hi_x == 0) {
        hi_x[0] = '0'; hi_x[1] = '\0';
    } else {
        memcpy(hi_x, x, len_hi_x); hi_x[len_hi_x] = '\0';
    }
    if (len_lo_x == 0) {
        lo_x[0] = '0'; lo_x[1] = '\0';
    } else {
        memcpy(lo_x, x + lx - len_lo_x, len_lo_x); lo_x[len_lo_x] = '\0';
    }
    strip_leading_zeroes(hi_x);
    strip_leading_zeroes(lo_x);

    /* Split y into high and low parts. */
    size_t len_lo_y = (ly <= k) ? ly : k;
    size_t len_hi_y = ly - len_lo_y;
    char hi_y[2105], lo_y[2105];
    if (len_hi_y == 0) {
        hi_y[0] = '0'; hi_y[1] = '\0';
    } else {
        memcpy(hi_y, y, len_hi_y); hi_y[len_hi_y] = '\0';
    }
    if (len_lo_y == 0) {
        lo_y[0] = '0'; lo_y[1] = '\0';
    } else {
        memcpy(lo_y, y + ly - len_lo_y, len_lo_y); lo_y[len_lo_y] = '\0';
    }
    strip_leading_zeroes(hi_y);
    strip_leading_zeroes(lo_y);

    /* Compute the sums hi+lo for each operand. */
    char sum1[2105], sum2[2105];
    add_str(hi_x, lo_x, sum1);
    add_str(hi_y, lo_y, sum2);
    strip_leading_zeroes(sum1);
    strip_leading_zeroes(sum2);

    /* Recurse on the three child pairs. */
    seq_t a = rec(hi_x, strlen(hi_x), hi_y, strlen(hi_y));
    seq_t b = rec(lo_x, strlen(lo_x), lo_y, strlen(lo_y));
    seq_t c = rec(sum1, strlen(sum1), sum2, strlen(sum2));

    return 1 + a + b + c;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;           /* undefined below offset, but harmless */
    }

    /* Static cache for Fibonacci numbers as decimal strings. */
    static char **fib_cache = NULL;
    static idx_t fib_cache_size = 0;

    /* Initialise the cache with F(0) = "0" and F(1) = "1". */
    if (fib_cache_size == 0) {
        fib_cache_size = 2;
        fib_cache = (char **)malloc(fib_cache_size * sizeof(char *));
        fib_cache[0] = dup_str("0");
        fib_cache[1] = dup_str("1");
    }

    /* Ensure we have Fibonacci numbers up to n+1 in the cache. */
    while (fib_cache_size <= n + 1) {
        /* Compute F(k) = F(k-1) + F(k-2). */
        size_t la = strlen(fib_cache[fib_cache_size - 1]);
        size_t lb = strlen(fib_cache[fib_cache_size - 2]);

        /* Allocate buffer for the sum (at most la+lb+1 digits). */
        char *sum = (char *)malloc(la + lb + 2);
        if (!sum) {
            abort();          /* memory failure */
        }

        add_str(fib_cache[fib_cache_size - 1], fib_cache[fib_cache_size - 2], sum);

        /* Expand the cache array and store the new Fibonacci number. */
        char **tmp = (char **)realloc(fib_cache, (fib_cache_size + 1) * sizeof(char *));
        if (!tmp) {
            abort();
        }
        fib_cache = tmp;
        fib_cache[fib_cache_size] = sum;
        ++fib_cache_size;
    }

    /* The answer is the number of Karatsuba calls for F(n) * F(n+1). */
    return rec(fib_cache[n],   strlen(fib_cache[n]),
               fib_cache[n+1], strlen(fib_cache[n+1]));
}
