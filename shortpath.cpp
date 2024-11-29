#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <cmath>
#include <vector>
#include <cstdlib>

using namespace std;

      struct place
      {
            int code;
            string state;
            string name;
            int popul;
            double area;
            double lat;
            double lon;
            int road;
            double dist;
      };

      struct state
      {
            string abbreviation;
            string statename;
      };

      struct node
      {
          place * data;
          node * next;

          node(place * p)
          {
              data = p;
              next = NULL;
          }
      };

    class hashtable
    {
          private:
              node ** table;
              int capacity;
              int size;

              unsigned int hashfunc(string s)
              {
                  unsigned int hash = 92439827;
                  for (unsigned int i = 0; i < s.length(); i++)
                  {
                      hash = hash * 169 + s[i];
                  }

                  return hash % capacity;
              }

              void resize()
              {
                  int old_cap = capacity;
                  capacity *= 2;
                  node ** new_table = new node * [capacity];

                  for (int i = 0; i < capacity; i++)
                  {
                      new_table[i] = NULL;
                  }

                  for (int i = 0; i < old_cap; i++)
                  {
                      node* current = table[i];
                      while (current != NULL)
                      {
                          unsigned int new_index = hashfunc(current->data->name);
                          node * next_node = current->next;

                          current->next = new_table[new_index];
                          new_table[new_index] = current;

                          current = next_node;
                      }
                  }

                  delete[] table;
                  table = new_table;
              }

          public:
              hashtable(int initial_cap)
              {
                  capacity = initial_cap;
                  size = 0;
                  table = new node * [capacity];

                  for (int i = 0; i < capacity; i++)
                  {
                      table[i] = NULL;
                  }
              }

              void add_place(place new_place)
              {
                    unsigned int index = hashfunc(new_place.name);
                    place * place_ptr = new place(new_place);
                    node * x = new node(place_ptr);

                    if (table[index] != NULL)
                    {
                        x->next = table[index];
                    }

                    table[index] = x;

                    size++;

                    if (size >= capacity)
                    {
                        resize();
                    }
              }

              place * search_place(string name, string state)
              {
                  unsigned int index = hashfunc(name);
                  node * x = table[index];

                  while (x != NULL)
                  {
                      if (x->data->name == name && x->data->state == state)
                      {
                          return x->data;
                      }
                      x = x->next;
                  }
                  return NULL;
              }

              vector<place *> get_states(string name)
              {
                  vector<place*> matching_places;
                  unsigned int index = hashfunc(name);

                  node * x = table[index];

                  while (x != NULL)
                  {
                       if (x->data != NULL && x->data->name == name)
                       {
                            matching_places.push_back(x->data);
                       }
                       x = x->next;
                  }
                  return matching_places;
              }
    };

    string delete_spaces(string name)
    {
        int start = 0;
        int end = name.length() - 1;

        while (end >=0 && name[end] == ' ')
        {
            end --;
        }

        return name.substr(start, end + 1);
    }

    vector<state> states_vector;

    void read_states(string filename)
    {
        ifstream stateslist(filename);

        if (!stateslist.is_open())
        {
             cout << "States file could not be opened." << endl;
             return;
        }

        string info;
        while (getline(stateslist, info))
        {
             state x;
             x.abbreviation = info.substr(0, 2);
             x.statename = info.substr(3, 45);
             x.statename = delete_spaces(x.statename);
             states_vector.push_back(x);
        }
    }

    void read_places(string filename, hashtable& ht)
    {
          ifstream placeslist(filename);

          if (!placeslist.is_open())
          {
                 cout<<"Problem reading file."<<endl;
                 return;
          }
                                                                                                                                                                                                                                                                                                                                                                                                  
          string info;

          while(getline(placeslist, info))
          {
                place x;
                x.code = stoi(info.substr(0, 8));
                x.state = info.substr(8, 2);
                x.name = info.substr(10, 45);
                x.popul = stoi(info.substr(56, 11));
                x.area = stod(info.substr(67, 14));
                x.lat = stod(info.substr(81, 10));
                x.lon = stod(info.substr(91, 11));
                x.road = stoi(info.substr(101, 6));
                x.dist = stod(info.substr(107, 7));

                x.name = delete_spaces(x.name);
                ht.add_place(x);
          }
    }

      const double pi = acos(-1.0);
      const string dirs[] = { "N", "NE", "E", "SE", "S", "SW", "W", "NW", "N" };

      string dirfromto(double longitude1, double latitude1, double longitude2, double latitude2)
      {
            double d = atan2(longitude2 - longitude1,
            latitude2 - latitude1) * 180.0 / pi + 22.5;
            if (d < 0)
                d += 360.0;
            int n = (int)(d / 45.0);
            return dirs[n];
      }

      struct intersection
      {
          double lon;
          double lat;
          double dist;
          string state;
          string city;

          intersection(string info)
          {
                  lon = stod(info.substr(0, 9));
                  lat = stod(info.substr(12, 7));
                  dist = stod(info.substr(21, 6));
                  state = info.substr(28, 2);
                  city = info.substr(31);
          }
      };

      vector<intersection *> intersection_list;

      void read_intersections(string filename)
      {
           ifstream interlist(filename);

            if (!interlist.is_open())
            {
                  cout << "Intersections file could not be opened." << endl;
                  return;
            }

            string info;
            while (getline(interlist, info))
            {
                  intersection * p = new intersection(info);
                  intersection_list.push_back(p);
            }
      }

      struct connection
      {
          string roadName;
          string roadType;
          int intA;
          int intB;
          double length;

          connection(string n, string t, int a, int b, double len)
          {
                roadName = n;
                roadType = t;
                intA = a;
                intB = b;
                length= len;
          }
      };

      vector<connection *> connection_list;

      void read_connections(string filename)
      {
          ifstream connectlist(filename);

          if (!connectlist.is_open())
          {
                cout << "Connections file could not be opened." << endl;
                return;
          }

          string n, t;
          int a, b;
          double len;
          while (true)
          {
                connectlist>>n>>t>>a>>b>>len;
                if(connectlist.fail())
                {
                    break;
                }    
                
                connection * p = new connection(n, t, a, b, len);
                connection_list.push_back(p);
          }
      }

      template <typename T>
      class PriorityQueue 
      {
           protected:           
           struct pqNode 
           {
                T data;
                int priority;

                pqNode(T d, int p) 
                {
                    data = d;
                    priority = p;
                }
           };

           vector<pqNode> heap;

           void heapify_up(int index)
           {
                 while (index > 0) 
                 {
                      int parentIndex = (index - 1) / 2;
                      if (heap[index].priority >= heap[parentIndex].priority) 
                      {
                          break;
                      }
                      swap(heap[parentIndex], heap[index]);
                      index = parentIndex;
                 }
           }

           
           void heapify_down(int index)
           {
                int size = heap.size();
                while (index < size) 
                {
                      int left = 2 * index + 1;
                      int right = 2 * index + 2;
                      int smallest = index;

                      if (left < size && heap[left].priority < heap[smallest].priority) 
                      {
                           smallest = left;
                      }

                      if (right < size && heap[right].priority < heap[smallest].priority) 
                      {
                           smallest = right;
                      }

                      if (smallest == index) 
                      {
                           break;
                      }

                      swap(heap[index], heap[smallest]);
                      index = smallest;
                }

           }

           public: 
           void push(T data, int priority)
           {
                heap.push_back(pqNode(data, priority));
                heapify_up(heap.size() - 1);
           }

           bool empty()
           {
                return heap.empty();
           }

           T top() 
           {
                return heap[0].data;
           }

           void pop() 
           {
                heap[0] = heap.back();
                heap.pop_back();
                heapify_down(0);
           }
      };
      
      vector<int> dijkstra(int start, int end)
      {
             int n = intersection_list.size();

             vector<double> distances(n, 1000000);
             vector<int> previous(n, -1);

             PriorityQueue<int> pq;

             pq.push(start, 0);
             distances[start] = 0;

             while (!pq.empty()) 
             {
                  int current = pq.top();
                  pq.pop();
                  
                  if (current == end)
                  {
                        break;
                  }

                  for (int i = 0; i < connection_list.size(); i++)
                  {
                        connection* conn = connection_list[i];
                        int other;

                        if (conn->intA == current) 
                        {
                            other = conn->intB;
                        } 
                        else if (conn->intB == current) 
                        {
                            other = conn->intA;
                        }
                        else 
                        {
                            continue;
                        }

                        double new_dist = distances[current] + conn->length;

                        if (new_dist < distances[other]) 
                        {
                              distances[other] = new_dist;
                              previous[other] = current;
                              pq.push(other, new_dist);
                        }
                  }
             }

             vector<int> path;
             int endInter = end;
             while (endInter != -1) 
             {
                  path.push_back(endInter);
                  endInter = previous[endInter];
             }

             for (int i = 0; i < path.size()/2; i++) 
             {
                  swap(path[i], path[path.size() - 1 - i]);
             }

             return path;
      }

      void print_path(vector<int>& path) 
      {
            if (path.empty()) 
            {
                cout << "No path available." << endl;
                return;
            }

            double total_distance = 0.0;
            string current_road = "";
            double current_road_length = 0.0;
            int prevInter = path[0];

            for (int i = 1; i < path.size(); i++) 
            {
                int currentIntersection = path[i];
                double road_distance = 0.0;
                string roadName;

                for (int j = 0; j < connection_list.size(); j++)
                {
                      if ((connection_list[j]->intA == prevInter && connection_list[j]->intB == currentIntersection) || (connection_list[j]->intB == prevInter && connection_list[j]->intA == currentIntersection)) 
                      {
                            roadName = connection_list[j]->roadName;
                            road_distance = connection_list[j]->length;
                            break;
                      }
                }
                
                if (roadName == current_road) 
                {
                     current_road_length += road_distance;
                }
                else 
                {
                      if (!current_road.empty()) 
                      {
                            string direction = dirfromto(intersection_list[prevInter]->lon, intersection_list[prevInter]->lat, intersection_list[currentIntersection]->lon, intersection_list[currentIntersection]->lat);
                            
                            cout<<"Take "<<roadName<<" "<<direction<<" "<<current_road_length<<" miles to intersection #"<<prevInter<<", "<<intersection_list[prevInter]->dist<<" miles from "<<intersection_list[prevInter]->city<<", "<<intersection_list[prevInter]->state<<endl;
                      }

                      current_road = roadName;
                      current_road_length = road_distance;
                 }
                  
                 prevInter = currentIntersection;
                 total_distance += road_distance;
            }    
            
            if (!current_road.empty()) 
            {
                string direction = dirfromto(intersection_list[path[path.size() - 2]]->lon, intersection_list[path[path.size() - 2]]->lat, intersection_list[path.back()]->lon, intersection_list[path.back()]->lat);
                
                cout<<"Take "<<current_road<<" "<<direction<<" "<<current_road_length<<" miles to intersection #"<<path.back()<<", "<<intersection_list[path.back()]->dist<<" miles from "<<intersection_list[path.back()]->city<<", " << intersection_list[path.back()]->state << endl;
            }   
            cout<<"Total "<<total_distance<<" miles"<<endl;
      }

      
      int main()
      {
            hashtable places(8);

            read_states("states.txt");
            read_places("/home/www/class/een318/named-places.txt", places);
            read_intersections("/home/www/class/een318/intersections.txt");
            read_connections("/home/www/class/een318/connections.txt");
            
            vector<connection*> connectionsForIntersection;
            string placename, statename;
            int currentInter = 0;
            int endInter = 0;
            place * first;
            place * second;
            int counter = 0;

            while (counter < 1)
            {
            cout<<"Enter a City: "<<endl;
            getline(cin, placename);

            cout<<"Possibilities: "<<endl;

            vector<place *> found_places1 = places.get_states(placename);
            
            for (int i = 0; i < found_places1.size(); i++)
            {
                  place * x = found_places1[i];
                        
                  for (int j = 0; j < states_vector.size(); j++)
                  {
                       if (states_vector[j].abbreviation == x->state)
                       {
                            cout<<states_vector[j].abbreviation<<" "<<endl;
                       }
                  }
            }
                           
                cout<<"Starting Place: "<<endl;
                getline(cin, statename);
                place * found_place1 = places.search_place(placename, statename);
                  
                if (found_place1 != NULL)
                {
                     first = found_place1;
                     currentInter = found_place1->road;
                     cout<<"Place is "<<found_place1->code<<" "<<found_place1->state<<" "<<found_place1->name<<" "<<found_place1->popul<<" "<<found_place1->area<<" "<<"("<<found_place1->lat<<", "<<found_place1->lon<<"), closest intersection "<<found_place1->road<<" "<<found_place1->dist<<endl;
                }
                else
                {
                      cout<<"Place does not exist"<<endl;
                      break;
                }
           

            cout<<"Enter a City: "<<endl;
            getline(cin, placename);

            cout<<"Possibilities: "<<endl;

            vector<place *> found_places = places.get_states(placename);

            for (int i = 0; i < found_places.size(); i++)
            {
                  place * x = found_places[i];

                  for (int j = 0; j < states_vector.size(); j++)
                  {
                       if (states_vector[j].abbreviation == x->state)
                       {
                            cout<<states_vector[j].abbreviation<<" "<<endl;
                       }
                  }
            }
 
                cout<<"Destination: "<<endl;
                getline(cin, statename);
                place * found_place = places.search_place(placename, statename);
            
                       if (found_place != NULL)
                       {
                            second = found_place;
                            endInter = found_place->road;
                            cout<<"Place is "<<found_place->code<<" "<<found_place->state<<" "<<found_place->name<<" "<<found_place->popul<<" "<<found_place->area<<" "<<"("<<found_place->lat<<", "<<found_place->lon<<"), closest intersection "<<found_place->road<<" "<<found_place->dist<<endl;
                       }
                       else
                       {
                            cout<<"Place does not exist"<<endl;
                            break;
                       }
           

            cout<<"Starting Point is Intersection #"<<currentInter<<", "<<first->dist<<" miles from "<<first->name<<", "<<first->state<<endl;

            vector<int> path = dijkstra(currentInter, endInter);

            print_path(path);            
            counter++;
      }
}