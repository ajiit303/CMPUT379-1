/** @brief Replace variable such $HOME with its actual content
 * 
 * @param t Input String
 * @param omitSet Omitted character such as "$"
 * @param repStr 
 * @return Void
 */
int gsub( char *t, char *omitSet, char *repStr );

/** @brief Split command and its arguments based on field separater
 *
 * @param inStr Input command line
 * @param token Arguments of command line (Max 32)
 * @param fs Field separater for separating arguments and command
 * @return Number of tokens
 */
int split( char inStr[], char token[][], char fs[]);



