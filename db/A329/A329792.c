/*
 * A329792  Smallest positive k such that k*n contains only the digits
 *          1,2,3,4,5, or -1 if no such k exists.
 * https://oeis.org/A329792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, base
 * Formula:   not stated
 * Range:     i8 n <= 801, i16 n <= 9730, i32 n <= 9730, i64 n <= 9730,
 *            i128 n <= 9730, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function implements the definition of sequence A329792:
 * a(n) is the smallest positive integer k such that k*n,
 * when written in decimal, uses only the digits 1,2,3,4,5.
 * If n is a multiple of 10 (or no such k exists) it returns -1.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0 || n % 10 == 0) {
        return -1;
    }

    /* Modulus for BFS (all residues are treated as unsigned indices) */
    idx_t mod = n;                     /* 0 < mod <= 10000 for our constraints */

    /* Allocate BFS tables */
    bool *visited  = (bool*)calloc(mod, sizeof(bool));
    int  *parent   = (int*)malloc(mod * sizeof(int));
    uint8_t *digit = (uint8_t*)malloc(mod * sizeof(uint8_t)); /* digit that leads to the residue */
    int *queue = (int*)malloc(mod * sizeof(int));

    if (!visited || !parent || !digit || !queue) {
        /* Memory allocation failure – not expected for given limits. */
        return -1;
    }

    int qh = 0, qt = 0;
    int found = -1;          /* remainder 0 (i.e., a multiple) when found */

    /* Initialise BFS with single‑digit candidates (1…5) */
    for (uint8_t d = 1; d <= 5; ++d) {
        int r = d % (int)mod;
        if (!visited[r]) {
            visited[r] = true;
            parent[r]  = -1;
            digit[r]   = d;
            queue[qt++] = r;
        }
        if (r == 0) {          /* single digit is already a multiple */
            seq_t res = (seq_t)(d / n); /* d = k*n → k = d/n */
            free(visited); free(parent); free(digit); free(queue);
            return res;
        }
    }

    /* BFS search */
    while (qh < qt) {
        int rem = queue[qh++];
        for (uint8_t d = 1; d <= 5; ++d) {
            int newRem = (int)((((uint64_t)rem * 10u) + d) % mod);
            if (!visited[newRem]) {
                visited[newRem] = true;
                parent[newRem]  = rem;
                digit[newRem]   = d;
                queue[qt++] = newRem;
                if (newRem == 0) {
                    found = 0;
                    break;
                }
            }
        }
        if (found == 0) break;
    }

    /* Reconstruct the allowed number (string of decimal digits) */
    char rev[10001];          /* max length will be <= mod (<=10000) */
    int len = 0;
    for (int cur = 0; cur != -1; cur = parent[cur]) {
        rev[len++] = '0' + digit[cur];
    }
    /* Reverse to obtain the correct order */
    char *numStr = (char*)malloc(len + 1);
    for (int i = 0; i < len; ++i) {
        numStr[i] = rev[len - 1 - i];
    }
    numStr[len] = '\0';

    /* Divide the constructed number by n to get k */
    char *quotStr = (char*)malloc(len + 1);
    int qpos = 0;
    int carry = 0;
    for (int i = 0; i < len; ++i) {
        int val = carry * 10 + (numStr[i] - '0');
        int qdig = val / (int)n;
        carry   = val % (int)n;
        if (qpos > 0 || qdig > 0) {   /* skip leading zeros */
            quotStr[qpos++] = '0' + qdig;
        }
    }
    if (qpos == 0) quotStr[qpos++] = '0';   /* safety */
    quotStr[qpos] = '\0';

    seq_t result = seq_from_str(quotStr);

    /* Cleanup */
    free(visited); free(parent); free(digit); free(queue);
    free(numStr); free(quotStr);

    return result;
}
