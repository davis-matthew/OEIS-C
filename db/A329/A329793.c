/*
 * A329793  Smallest positive multiple of n that contains only the digits
 *          1,2,3,4,5, or -1 if no such multiple exists.
 * https://oeis.org/A329793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 28, i16 n <= 598, i32 n <= 9829, i64 n <= 9829,
 *            i128 n <= 9829, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A329793:  Smallest positive multiple of n that contains only the
    digits 1,2,3,4,5; return -1 if no such multiple exists.
    Computed by a breadth‑first search over the remainders modulo n. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool *visited = NULL;
static idx_t *parent = NULL;
static unsigned char *digit = NULL;
static idx_t *queue   = NULL;
static size_t cap = 0;

static void ensure_capacity(idx_t n)
{
    if (n <= cap)
        return;
    /* grow by at least 16 for small n, otherwise support up to n */
    size_t newcap = n + 16;
    visited = (bool *)realloc(visited, newcap * sizeof(bool));
    parent  = (idx_t *)realloc(parent,  newcap * sizeof(idx_t));
    digit   = (unsigned char *)realloc(digit, newcap * sizeof(unsigned char));
    queue   = (idx_t *)realloc(queue,  newcap * sizeof(idx_t));
    cap = newcap;
}

/* Main function as required */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0)
        return (seq_t)-1;
    if (n % 10 == 0)
        return (seq_t)-1;

    ensure_capacity(n);
    memset(visited, 0, n * sizeof(bool));

    size_t head = 0, tail = 0;

    /* enqueue all one‑digit candidates (1..5) */
    for (unsigned d = 1; d <= 5; ++d) {
        idx_t r = d % n;
        if (visited[r])
            continue;
        visited[r] = true;
        parent[r] = (idx_t)-1;          /* root marker */
        digit[r] = (unsigned char)d;
        queue[tail++] = r;
        if (r == 0)
            return (seq_t)d;
    }

    /* BFS */
    while (head < tail) {
        idx_t cur = queue[head++];
        for (unsigned d = 1; d <= 5; ++d) {
            idx_t next = (cur * 10 + d) % n;
            if (visited[next])
                continue;
            visited[next] = true;
            parent[next] = cur;
            digit[next] = (unsigned char)d;
            if (next == 0) {
                /* reconstruct the number */
                unsigned char rev[20000];   /* enough for n < 2e4 */
                size_t len = 0;
                idx_t r = 0;
                while (true) {
                    rev[len++] = digit[r];
                    idx_t p = parent[r];
                    if (p == (idx_t)-1)
                        break;
                    r = p;
                }
                seq_t result = 0;
                for (long i = (long)len - 1; i >= 0; --i)
                    result = result * 10 + (seq_t)rev[i];
                return result;
            }
            queue[tail++] = next;
        }
    }

    /* should never happen for n not divisible by 10 */
    return (seq_t)-1;
}
